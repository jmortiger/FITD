#ifndef _TRACK_
#define _TRACK_
/// @brief Speed when player is running forwards
#define RUNNING_SPEED 5
/// @brief Speed when player is walking forwards
#define WALKING_SPEED 4
/// @brief Speed when player is walking backwards
#define RETREAT_SPEED -1

void processTrack(void);
void processTrack2(void);
/// @brief Handle manual rotation
/// @param param 
void GereManualRot(int param);

#endif
