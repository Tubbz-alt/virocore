#include "ARDeclarativeNode_JNI.h"
#include <VROPlatformUtil.h>
#include "ARUtils_JNI.h"

#if VRO_PLATFORM_ANDROID
#define VRO_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
      Java_com_viro_core_internal_ARDeclarativeNode_##method_name
#endif

extern "C" {

VRO_METHOD(void, nativeSetAnchorId) (VRO_ARGS
                                     VRO_REF node_j,
                                     VRO_STRING id_j) {
    std::string id_s = VROPlatformGetString(id_j, env);
    std::shared_ptr<VROARDeclarativeNode> node = ARDeclarativeNode::native(node_j);
    node->setAnchorId(id_s);
}

VRO_METHOD(VRO_REF, nativeCreateARNodeDelegate) (VRO_ARGS
                                                 VRO_REF node_j) {
    std::shared_ptr<ARDeclarativeNodeDelegate> delegate = std::make_shared<ARDeclarativeNodeDelegate>(obj, env);
    std::shared_ptr<VROARDeclarativeNode> node = ARDeclarativeNode::native(node_j);
    node->setARNodeDelegate(delegate);
    return ARDeclarativeNodeDelegate::jptr(delegate);
}

VRO_METHOD(void, nativeDestroyARNodeDelegate) (VRO_ARGS
                                               VRO_REF delegateRef) {
    delete reinterpret_cast<PersistentRef<ARDeclarativeNodeDelegate> *>(delegateRef);
}

}

void ARDeclarativeNodeDelegate::onARAnchorAttached(std::shared_ptr<VROARAnchor> anchor) {
    std::weak_ptr<VROARAnchor> anchor_w = std::dynamic_pointer_cast<VROARAnchor>(anchor);
    JNIEnv *env = VROPlatformGetJNIEnv();
    jweak jObject_w = env->NewWeakGlobalRef(_javaObject);
    VROPlatformDispatchAsyncApplication([this, jObject_w, anchor_w] {
        JNIEnv *env = VROPlatformGetJNIEnv();
        VRO_OBJECT localObj = env->NewLocalRef(jObject_w);
        std::shared_ptr<VROARAnchor> anchor_s = anchor_w.lock();
        if (localObj == NULL || !anchor_s) {
            env->DeleteWeakGlobalRef(jObject_w);
            return;
        }

        VRO_OBJECT anchorObj = ARUtilsCreateJavaARAnchorFromAnchor(anchor_s);
        VROPlatformCallJavaFunction(localObj, "onAnchorFound", "(Lcom/viro/core/ARAnchor;)V",
                                    anchorObj);
        env->DeleteWeakGlobalRef(jObject_w);
    });
}

void ARDeclarativeNodeDelegate::onARAnchorUpdated(std::shared_ptr<VROARAnchor> anchor) {
    std::weak_ptr<VROARAnchor> anchor_w = std::dynamic_pointer_cast<VROARAnchor>(anchor);
    JNIEnv *env = VROPlatformGetJNIEnv();
    jweak jObject_w = env->NewWeakGlobalRef(_javaObject);
    VROPlatformDispatchAsyncApplication([this, jObject_w, anchor_w] {
        JNIEnv *env = VROPlatformGetJNIEnv();
        VRO_OBJECT localObj = env->NewLocalRef(jObject_w);
        std::shared_ptr<VROARAnchor> anchor_s = anchor_w.lock();
        if (localObj == NULL || !anchor_s) {
            env->DeleteWeakGlobalRef(jObject_w);
            return;
        }

        VRO_OBJECT anchorObj = ARUtilsCreateJavaARAnchorFromAnchor(anchor_s);
        VROPlatformCallJavaFunction(localObj, "onAnchorUpdated", "(Lcom/viro/core/ARAnchor;)V",
                                    anchorObj);
        env->DeleteWeakGlobalRef(jObject_w);
    });
}

void ARDeclarativeNodeDelegate::onARAnchorRemoved() {
    JNIEnv *env = VROPlatformGetJNIEnv();
    jweak jObject_w = env->NewWeakGlobalRef(_javaObject);
    VROPlatformDispatchAsyncApplication([jObject_w] {
        JNIEnv *env = VROPlatformGetJNIEnv();
        VRO_OBJECT localObj = env->NewLocalRef(jObject_w);
        if (localObj == NULL) {
            env->DeleteWeakGlobalRef(jObject_w);
            return;
        }

        VROPlatformCallJavaFunction(localObj, "onAnchorRemoved", "()V");
        env->DeleteWeakGlobalRef(jObject_w);
    });
}