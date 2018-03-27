#ifndef  __DOOR_STATUS_TASK_H__
#define  __DOOR_STATUS_TASK_H__


extern osThreadId door_status_task_hdl;
void door_status_task(void const * argument);

/*获取门的状态*/
uint8_t door_status_task_get_door_status();

#define  DOOR_STATUS_TASK_INTERVAL                     500/*门状态监测间隔500ms*/


#define  DOOR_STATUS_TASK_DOOR_STATUS_OPEN             1
#define  DOOR_STATUS_TASK_DOOR_STATUS_CLOSE            2
#define  DOOR_STATUS_TASK_DOOR_STATUS_INIT             3




#endif