/**
 * @module cocos2dx_spine
 */
var sp = sp || {};

/**
 * @class Skeleton
 */
sp.Skeleton = {

/**
 * @method setToSetupPose
 */
setToSetupPose : function (
)
{
},

/**
 * @method setBlendFunc
 * @param {cc.BlendFunc} arg0
 */
setBlendFunc : function (
blendfunc 
)
{
},

/**
 * @method onDraw
 * @param {kmMat4} arg0
 * @param {bool} arg1
 */
onDraw : function (
kmmat4, 
bool 
)
{
},

/**
 * @method setSlotsToSetupPose
 */
setSlotsToSetupPose : function (
)
{
},

/**
 * @method setAttachment
 * @param {char} arg0
 * @param {char} arg1
 * @return {bool}
 */
setAttachment : function (
char, 
char 
)
{
    return false;
},

/**
 * @method getBlendFunc
 * @return {cc.BlendFunc}
 */
getBlendFunc : function (
)
{
    return cc.BlendFunc;
},

/**
 * @method setSkin
 * @param {char} arg0
 * @return {bool}
 */
setSkin : function (
char 
)
{
    return false;
},

/**
 * @method updateWorldTransform
 */
updateWorldTransform : function (
)
{
},

/**
 * @method getLocalBounds
 * @return {RectObject}
 */
getLocalBounds : function (
)
{
    return cc.Rect;
},

/**
 * @method setBonesToSetupPose
 */
setBonesToSetupPose : function (
)
{
},

/**
 * @method createWithFile
* @param {char|char} char
* @param {char|spAtlas} char
* @param {float|float} float
* @return {sp.Skeleton|sp.Skeleton}
*/
createWithFile : function(
char,
spatlas,
float 
)
{
    return sp.Skeleton;
},

/**
 * @method Skeleton
 * @constructor
* @param {char|spSkeletonData|char} char
* @param {spAtlas|bool|char} spatlas
* @param {float|float} float
*/
Skeleton : function(
char,
char,
float 
)
{
},

};

/**
 * @class SkeletonAnimation
 */
sp.SkeletonAnimation = {

/**
 * @method update
 * @param {float} arg0
 */
update : function (
float 
)
{
},

/**
 * @method setMix
 * @param {char} arg0
 * @param {char} arg1
 * @param {float} arg2
 */
setMix : function (
char, 
char, 
float 
)
{
},

/**
 * @method clearTracks
 */
clearTracks : function (
)
{
},

/**
 * @method clearTrack
 */
clearTrack : function (
)
{
},

/**
 * @method createWithFile
* @param {char|char} char
* @param {char|spAtlas} char
* @param {float|float} float
* @return {sp.SkeletonAnimation|sp.SkeletonAnimation}
*/
createWithFile : function(
char,
spatlas,
float 
)
{
    return sp.SkeletonAnimation;
},

/**
 * @method SkeletonAnimation
 * @constructor
* @param {char|spSkeletonData|char} char
* @param {spAtlas|char} spatlas
* @param {float|float} float
*/
SkeletonAnimation : function(
char,
char,
float 
)
{
},

};

/**
 * @class JSSkeletonAnimation
 */
sp.JSSkeletonAnimation = {

/**
 * @method createWithFile
* @param {char|char} char
* @param {char|spAtlas} char
* @param {float|float} float
* @return {sp.JSSkeletonAnimation|sp.JSSkeletonAnimation}
*/
createWithFile : function(
char,
spatlas,
float 
)
{
    return sp.JSSkeletonAnimation;
},

/**
 * @method JSSkeletonAnimation
 * @constructor
* @param {char|char} char
* @param {char|spAtlas} char
* @param {float|float} float
*/
JSSkeletonAnimation : function(
char,
spatlas,
float 
)
{
},

};
