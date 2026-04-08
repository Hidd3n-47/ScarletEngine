#include "MutableEntityHandle.h"

#include "Scene.h"

namespace ScarlEnt
{

void MutableEntityHandle::DestroyEntity()
{
#ifdef DEV_CONFIGURATION
    SCARLENT_ASSERT(mIsValid && "Trying to remove entity on mutable entity that has been marked as invalid.");

    mParent->RemoveMutableHandle(this);
#endif // DEV_CONFIGURATION.
    mIsValid = false;

    mComponentManagerRef->RemoveMutableEntity(mEntityId.runtimeId);
}

} // Namespace ScarlEnt.