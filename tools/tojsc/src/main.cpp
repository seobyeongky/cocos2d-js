//
//  main.cpp
//  jscompiler
//
//  Created by 서병기 on 2014. 10. 6..
//  Copyright (c) 2014년 redduck. All rights reserved.
//
#include <stdio.h>
#include <dirent.h>
#include <jsapi.h>
#include <string>
#include <vector>


static void sc_finalize(JSFreeOp *freeOp, JSObject *obj) {
    printf("jsbindings: finalizing JS object %p (global class)\n", obj);
}

static JSClass global_class = {
    "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_DeletePropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, sc_finalize,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

JSObject* NewGlobalObject(JSContext* cx)
{
    JS::CompartmentOptions options;
    options.setVersion(JSVERSION_LATEST);
    
    JS::RootedObject glob(cx, JS_NewGlobalObject(cx, &global_class, NULL, JS::DontFireOnNewGlobalHook, options));
    if (!glob) {
        return NULL;
    }
    JSAutoCompartment ac(cx, glob);
    bool ok = true;
    ok = JS_InitStandardClasses(cx, glob);
    if (ok)
        JS_InitReflect(cx, glob);
//    if (ok && debug)
//        ok = JS_DefineDebuggerObject(cx, glob);
    if (!ok)
        return NULL;
    
    JS_FireOnNewGlobalObject(cx, glob);
    
    return glob;
}

bool tojsc(JSContext * cx, JSObject * global, const char * jspath, char * errmsg)
{
    JS::RootedScript script(cx);
    JS::RootedObject obj(cx, global);
    
    JS::CompileOptions options(cx);
    options.setUTF8(true).setFileAndLine(jspath, 1);
    script = JS::Compile(cx, obj, options, jspath);
    if (!script)
    {
        sprintf(errmsg, "compile failed");
        return false;
    }
    
    uint32_t length;
    void * data = JS_EncodeScript(cx, script, &length);
    std::string outpath(jspath);
    outpath += 'c';
    FILE * out = fopen(outpath.c_str(), "wb");
    if (!out)
    {
        sprintf(errmsg, "failed to open %s", outpath.c_str());
        return false;
    }
    fwrite(data, 1, length, out);
    fclose(out);

    return true;
}

#define ENTRY_PATH std::string entrypath(root);\
    entrypath += '/';\
    entrypath += dir_entry->d_name;

std::string last_slash_excluded(const std::string & text)
{
    if (text.back() == '/')
        return std::string(text.begin(), text.end() - 1);
    else
        return text;
}

void search_r(const std::string root, std::vector<std::string> * paths)
{
    std::vector<std::string> nexts;
    DIR * dir_info = opendir(root.c_str());
    if (dir_info == NULL)
    {
        printf("Failed to open directory %s\n", root.c_str());
        return;
    }
    dirent * dir_entry;
    while (1)
    {
        dir_entry = readdir(dir_info);
        if (!dir_entry) break;

        uint16_t namlen = strlen(dir_entry->d_name);
        char last_0 = dir_entry->d_name[namlen - 3];
        char last_1 = dir_entry->d_name[namlen - 2];
        char last_2 = dir_entry->d_name[namlen - 1];
        if (last_0 == '.' && last_1 == 'j' && last_2 == 's')
        {
            ENTRY_PATH
            paths->push_back(entrypath);
        }
        else
        {
            if (dir_entry->d_type == DT_DIR && strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, ".."))
            {
                ENTRY_PATH
                nexts.push_back(entrypath);
            }
        }
//        printf("%s : %d\n", dir_entry->d_name, dir_entry->d_type);
    }
    closedir(dir_info);
    
    for (auto & next : nexts)
    {
        search_r(next, paths);
    }
}

int main(int argc, const char * argv[])
{
    if (argc != 2)
    {
        puts("Usage : tojsc <directory>");
        exit(1);
    }
    
    std::vector<std::string> paths;
    search_r(last_slash_excluded(argv[1]), &paths);

    JS_Init();
    JSRuntime * rt = JS_NewRuntime(8L * 1024L * 1024L, JS_USE_HELPER_THREADS);
    JS_SetGCParameter(rt, JSGC_MAX_BYTES, 0xffffffff);
    JSContext * cx = JS_NewContext(rt, 8192);
    {
        JS::ContextOptionsRef(cx).setTypeInference(true)
            .setIon(true)
            .setBaseline(true)
            .setAsmJS(true);

        JS::CompileOptions compileOptions(cx);
        compileOptions.setCompileAndGo(true);

        JS_SetGCParameter(rt, JSGC_MODE, JSGC_MODE_INCREMENTAL);
        JSObject * global = NewGlobalObject(cx);
        
        JSAutoCompartment ac(cx, global);
        for (auto & path : paths)
        {
            char errmsg[50];
            if (!tojsc(cx, global, path.c_str(), errmsg))
            {
                printf("%s : failed\n\treason : %s\n", path.c_str(), errmsg);
            }
            else
            {
                printf("%s : succeeded\n", path.c_str());
            }
        }
    }

    JS_DestroyContext(cx);
    JS_DestroyRuntime(rt);

    return 0;
}
