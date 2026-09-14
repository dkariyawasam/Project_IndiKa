#include "global.h"
#include "gflib.h"
#include "event_object_movement.h"
#include "field_item_animation.h"
#include "field_player_avatar.h"
#include "script.h"
#include "constants/event_object_movement.h"

static void Task_FieldItemAnimation(u8 taskId);

// Keep animation state separate from the item's task data and script variables.
void StartFieldItemAnimation(u8 itemTaskId, TaskFunc continuation)
{
    u8 taskId = CreateTask(Task_FieldItemAnimation, 8);
    gTasks[taskId].data[5] = itemTaskId;
    SetWordTaskArg(taskId, 8, (u32)continuation);
    if (itemTaskId != TASK_NONE)
        gTasks[itemTaskId].func = TaskDummy;
}

// Script callers retain their own lock and resume after waitstate.
void PlayFieldItemAnimation(void)
{
    StartFieldItemAnimation(TASK_NONE, NULL);
}

static void Task_FieldItemAnimation(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    struct ObjectEvent *player = &gObjectEvents[gPlayerAvatar.objectEventId];
    TaskFunc continuation;
    u8 itemTaskId;

    if (task->data[0] == 0)
    {
        task->data[1] = player->graphicsId;
        task->data[2] = player->facingDirection;
        task->data[3] = gPlayerAvatar.preventStep;
        task->data[4] = ArePlayerFieldControlsLocked();
        LockPlayerFieldControls();
        gPlayerAvatar.preventStep = TRUE;
        task->data[0] = 1;
    }
    if (task->data[0] == 1)
    {
        if (ObjectEventIsMovementOverridden(player)
         && !ObjectEventClearHeldMovementIfFinished(player))
            return;
        StartPlayerAvatarSummonMonForFieldMoveAnim();
        ObjectEventSetHeldMovement(player, MOVEMENT_ACTION_START_ANIM_IN_DIRECTION);
        task->data[0] = 2;
        return;
    }
    if (!ObjectEventClearHeldMovementIfFinished(player))
        return;

    ObjectEventSetGraphicsId(player, task->data[1]);
    ObjectEventTurn(player, task->data[2]);
    gPlayerAvatar.preventStep = task->data[3];
    if (!task->data[4])
        UnlockPlayerFieldControls();
    itemTaskId = task->data[5];
    continuation = (TaskFunc)GetWordTaskArg(taskId, 8);
    DestroyTask(taskId);
    if (itemTaskId == TASK_NONE)
        ScriptContext_Enable();
    else
    {
        gTasks[itemTaskId].func = continuation;
        continuation(itemTaskId);
    }
}
