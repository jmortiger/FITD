#include "common.h"

int getPosRelTable[] = { 4,1,8,2,4,1,8,0 };

int getMatrix(int param1, int actorIdx, int param2)
{
	unsigned char* matrixPtr = (unsigned char*)HQR_Get(listMatrix, param1);

	int matrixWidth = *matrixPtr++;
	int matrixHeigh = *matrixPtr++;

	matrixPtr += (objectTable[actorIdx].hardMat - 1) * matrixWidth;
	matrixPtr += (objectTable[param2].hardMat - 1);

	if (g_gameId == AITD3) {
		return *(unsigned char*)matrixPtr;
	}
	return *(char*)matrixPtr;
}

/// @brief 
/// @param actor1 
/// @param actor2 
/// @return 
/// @todo Change to be more self-documenting.
/// @details Evaluates the position of the current actor (the one that executes the script) relative to another actor specified in the parameter. Note that the angle of the current actor is considered, not only the position.
/// 
/// The possible values are: 0 (any), 1 (left), 2 (right), 4 (front) and 8 (back).
int getPosRel(tObject* actor1, tObject* actor2)
{
	int beta1 = actor1->beta;
	int counter = 3;
	ZVStruct localZv;
	int centerX;
	int centerZ;

	if (beta1 >= 0x80 && beta1 < 0x180) {
		counter = 2;
	}

	if (beta1 >= 0x180 && beta1 < 0x280) {
		counter = 1;
	}

	if (beta1 >= 0x280 && beta1 < 0x380) {
		counter = 0;
	}


	copyZv(&actor2->zv, &localZv);

	if (actor1->room != actor2->room) {
		getZvRelativePosition(&localZv, actor2->room, actor1->room);
	}

	centerX = (localZv.ZVX1 + localZv.ZVX2) / 2;
	centerZ = (localZv.ZVZ1 + localZv.ZVZ2) / 2;

	if (actor1->zv.ZVZ2 >= centerZ && actor1->zv.ZVZ1 <= centerZ) {
		if (actor1->zv.ZVX2 < centerX) {
			counter++;
		} else {
			if (actor1->zv.ZVX1 <= centerX) {
				return(0);
			} else {
				counter += 3;
			}
		}
	} else
		if (actor1->zv.ZVX2 >= centerX || actor1->zv.ZVX1 <= centerX) {
			if (actor1->zv.ZVZ2 < centerZ) {
				counter += 2;
			} else {
				if (actor1->zv.ZVZ1 <= centerZ) {
					return(0);
				}
			}
		} else {
			return(0);
		}

	return(getPosRelTable[counter]);
}

int calcDist(int X1, int Y1, int Z1, int X2, int Y2, int Z2)
{
	int xDist = abs(X1 - X2);
	int yDist = abs(Y1 - Y2);
	int zDist = abs(Z1 - Z2);

	return(xDist + yDist + zDist); // recheck overflow
}

int testZvEndAnim(tObject* actorPtr, char* animPtr, int param)
{
	s16 var_16;
	s16 var_14;
	s16 var_E = 0;
	s16 var_12 = 0;
	s16 var_10 = param;
	s16 var_18;
	ZVStruct localZv;

	ASSERT(actorPtr);
	ASSERT(animPtr);

	var_16 = *(s16*)(animPtr);
	animPtr += 2;
	var_14 = *(s16*)(animPtr);
	animPtr += 2;

	for (var_18 = 0; var_18 < var_16; var_18++) {
		animPtr += 2;
		var_12 += *(s16*)animPtr;
		animPtr += 2;
		animPtr += 2;
		var_E += *(s16*)animPtr; // step depth
		animPtr += 2;

		animPtr += var_14 * 8;
	}

	copyZv(&actorPtr->zv, &localZv);

	walkStep(0, var_E, actorPtr->beta);

	localZv.ZVX1 += animMoveX;
	localZv.ZVX2 += animMoveX;
	localZv.ZVY1 += var_10;
	localZv.ZVY2 += var_10;
	localZv.ZVZ1 += animMoveZ;
	localZv.ZVZ2 += animMoveZ;

	if (AsmCheckListCol(&localZv, &roomDataTable[actorPtr->room])) {
		return(0);
	}

	localZv.ZVY1 += 100;
	localZv.ZVY2 += 100;

	if (AsmCheckListCol(&localZv, &roomDataTable[actorPtr->room])) {
		return(1);
	}

	return(0);
}

/// @brief Prints the given function name, parameter, & value in the standard fashion & returns the given value.
/// @param param 
/// @param val 
/// @param name 
/// @return `val`
int _printFuncReturn(int param, int val, const char* name) {
	appendFormatted("%s(%i):%i ", name, param, val);
	return(val);
}
/// @brief Prints the given name alias & value in the standard fashion & returns the given value.
/// @param val 
/// @param name 
/// @return `val`
int _printReturn(int val, const char* name = NULL) {
	appendFormatted("%s:%i ", name, val);
	return(val);
}
/// @brief Gets the `indexInWorld` of the object at the specified index in `objectTable`, while outputting to `appendFormatted`.
/// @details A simple way to strip repeated code from `evalVar` & standardize/change output formatting w/o editing a million switch cases.
/// @param idx 
/// @param name 
/// @return -1 if `idx` was -1, `objectTable[idx].indexInWorld` otherwise.
int _worldIdxFromObjTbl(int idx, const char* name = NULL) {
	return _printReturn(idx != -1 ? objectTable[idx].indexInWorld : idx, name);
}

/// @brief 
/// @param name 
/// @return 
/// @todo Eliminate overlap w/ evalVar2
int evalVar(const char* name)
{
	if (g_gameId >= JACK) { return evalVar2(name); }

	int var1 = *(s16*)(currentLifePtr);
	currentLifePtr += 2;

	if (var1 == -1) {
		int temp = *(s16*)(currentLifePtr);
		currentLifePtr += 2;

		if (name)
			appendFormatted("%s:", name);
		appendFormatted("%d ", temp);

		return(temp);
	} else if (var1 == 0) {
		int temp = *(s16*)(currentLifePtr);
		currentLifePtr += 2;

		if (name)
			appendFormatted("%s:", name);
		// appendFormatted("vars[%d] <%i>, ", temp, vars[temp]);
		appendFormatted("vars[%d]:%s:%i ", temp, varsNameTable != NULL ? varsNameTable[temp] : "", vars[temp]);

		return(vars[temp]);
	} else {
		tObject* actorPtr = currentLifeActorPtr;
		int actorIdx = currentLifeActorIdx;

		if (var1 & 0x8000) {
			int objectNumber;

			objectNumber = *(s16*)currentLifePtr;

			actorIdx = ListWorldObjets[objectNumber].objIndex;

			currentLifePtr += 2;
			actorPtr = &objectTable[actorIdx];

			if (actorIdx == -1) {
				switch (var1 & 0x7FFF) {
					case 0x1F:
					{
						if (name) appendFormatted("%s:", name);
						appendFormatted("worldObjects[%d].room:%hi ", objectNumber, ListWorldObjets[objectNumber].room);
						return(ListWorldObjets[objectNumber].room);
					}
					case 0x26: // 0x24 for later games
					{
						if (name) appendFormatted("%s:", name);
						appendFormatted("worldObjects[%d].stage:%hi ", objectNumber, ListWorldObjets[objectNumber].stage);
						return(ListWorldObjets[objectNumber].stage);
					}
					default:
					{
						DebugPrintfLn(debugLevelEnum::DBO_L_ERROR, "Unsupported evalVar %X when actor not in room !\n", var1 & 0x7FFF);
						FITD_throwFatal(); // assert(0);
						return -1;
					}
				}
			}
		}
		{
			var1 &= 0x7FFF;

			var1--;

			if (name) appendFormatted("%s:", name);
			switch (var1) {
				case 0x0: // ACTOR_COLLIDER/COL[0]
				{
					return _worldIdxFromObjTbl(actorPtr->COL[0], "actor_collider");
				}
				case 0x1: // HARD_DEC/TRIGGER_COLLIDER
				{
					return _printReturn(actorPtr->HARD_DEC, "trigger_collider");
				}
				case 0x2: // HARD_COLLIDER
				{
					return _printReturn(actorPtr->HARD_COL, "hard_collider");
				}
				case 0x3: // HIT
				{
					return _worldIdxFromObjTbl(actorPtr->HIT, "hit");
				}
				case 0x4: // HIT_BY
				{
					return _worldIdxFromObjTbl(actorPtr->HIT_BY, "actor_collider");
				}
				case 0x5: // ANIM
				{
					return _printReturn(actorPtr->ANIM, "anim");
				}
				case 0x6: // END_ANIM
				{
					return _printReturn(actorPtr->END_ANIM, "end_anim");
				}
				case 0x7: // FRAME
				{
					return _printReturn(actorPtr->FRAME, "frame");
				}
				case 0x8: // END_FRAME
				{
					return _printReturn(actorPtr->END_FRAME, "end_frame");
				}
				case 0x9: // BODY
				{
					return _printReturn(actorPtr->bodyNum, "body");
				}
				case 0xA: // MARK
				{
					return _printReturn(actorPtr->MARK, "mark");
				}
				case 0xB: // NUM_TRACK
				{
					return _printReturn(actorPtr->trackNumber, "num_track");
				}
				case 0xC: // TODO: CHRONO; Why recheck?
				{
					return _printReturn(evalChrono(&actorPtr->CHRONO) / 60, "chrono"); // recheck
				}
				case 0xD: // TODO: ROOM_CHRONO; Why recheck?
				{
					return _printReturn(evalChrono(&actorPtr->ROOM_CHRONO) / 60, "room_chrono"); // recheck
				}
				case 0xE: // DIST
				{
					// TODO: Better output?
					int actorNumber = ListWorldObjets[*(s16*)currentLifePtr].objIndex;
					currentLifePtr += 2;

					int val = 32000; // TODO: Why is this value given when the actor is invalid?
					if (actorNumber != -1) {
						int tempX = objectTable[actorNumber].worldX;
						int tempY = objectTable[actorNumber].worldY;
						int tempZ = objectTable[actorNumber].worldZ;
						val = calcDist(actorPtr->worldX, actorPtr->worldY, actorPtr->worldZ, tempX, tempY, tempZ);
					}
					return _printFuncReturn(actorNumber, val, "dist");
				}
				case 0xF: // COL_BY
				{
					return _worldIdxFromObjTbl(actorPtr->COL_BY, "col_by");
				}
				case 0x10: // FOUND
				{
					// NOTE: Currently doesn't route output to a standardized method. Will need to be changed if output formatting changes.
					// NOTE: Nested `evalVar` call; watch for output oddities.
					appendFormatted("isFound(" ANSI_FG_DARK_GREY);
					int retVal = ListWorldObjets[evalVar()].flags2 & 0x8000 ? 1 : 0;
					appendFormatted(ANSI_RESET "):%i", retVal);
					return retVal;
				}
				case 0x11: // ACTION
				{
					return _printReturn(action, "action");
				}
				case 0x12: // POSREL
				{
					int objNum = *(s16*)currentLifePtr;
					currentLifePtr += 2;

					int retVal = 0;
					if (ListWorldObjets[objNum].objIndex != -1) {
						retVal = getPosRel(actorPtr, &objectTable[ListWorldObjets[objNum].objIndex]);
					}

					return _printFuncReturn(objNum, retVal, "posrel");
				}
				case 0x13: // keyboard_input
				{
					// TODO: Validate priority & inability to be more than one.
					// TODO: Output actual value along with returned value?
					int retVal = 0;
					if (localJoyD & 4)
						retVal = 4;
					if (localJoyD & 8)
						retVal = 8;
					if (localJoyD & 1)
						retVal = 1;
					if (localJoyD & 2)
						retVal = 2;

					return _printReturn(retVal, "keyboard_input");
				}
				case 0x14: // SPACE (Set to 1 if you hold SPACE or numpad 0.)
				{
					return _printReturn(localClick, "space");
				}
				case 0x15: // CONTACT (Returns COL[0] or (if empty) COL_BY)
				{
					int temp = actorPtr->COL[0];
					if (temp == -1) temp = actorPtr->COL_BY;
					return _worldIdxFromObjTbl(temp, "contact");
				}
				case 0x16: // ALPHA
				{
					return _printReturn(actorPtr->alpha, "alpha");
				}
				case 0x17: // BETA
				{
					return _printReturn(actorPtr->beta, "beta");
				}
				case 0x18: // GAMMA
				{
					return _printReturn(actorPtr->gamma, "gamma");
				}
				case 0x19: // INHAND
				{
					return _printReturn(inHandTable[currentInventory], "inhand");
				}
				case 0x1A: // HIT_FORCE
				{
					return _printReturn(actorPtr->hitForce, "hitforce");
				}
				case 0x1B: // CAMERA (used in L469 for the spider)
				{
					return _printReturn(*(u16*)(((currentCamera + 6) * 2) + cameraPtr), "camera");
				}
				case 0x1C: // RAND(ceil) (I think)
				{
					int temp = *(s16*)currentLifePtr;
					currentLifePtr += 2;
					int val = rand() % temp;
					return _printFuncReturn(temp, val, "rand");
				}
				case 0x1D: // FALLING
				{
					return _printReturn(actorPtr->falling, "falling");
				}
				case 0x1E: // ROOM
				{
					return _printReturn(actorPtr->room, "room");
				}
				case 0x1F: // LIFE
				{
					return _printReturn(actorPtr->life, "life");
				}
				case 0x20: // OBJECT(id) (I think)
				{
					int objNum = *(s16*)currentLifePtr;
					currentLifePtr += 2;
					int retVal = ListWorldObjets[objNum].flags2 & 0xC000 ? 1 : 0;
					return _printFuncReturn(objNum, retVal, "object");
				}
				case 0x21: // ROOMY
				{
					return _printReturn(actorPtr->roomY, "room_y");
				}
				case 0x22: // TODO: TEST_ZV_END_ANIM? What is that? It's used once in the scripts
				{
					// NOTE: Currently doesn't route output to a standardized method. Will need to be changed if output formatting changes.
					int animNum = *(s16*)currentLifePtr;
					currentLifePtr += 2;
					int temp = *(s16*)currentLifePtr;
					currentLifePtr += 2;

					int retVal = testZvEndAnim(actorPtr, HQR_Get(listAnim, animNum), temp);
					appendFormatted("test_zv_end_anim(anim:%i,%i):%i ", animNum, temp, retVal);
					return(retVal);
				}
				case 0x23: // TODO: music; Used to check if the correct music is playing to appease the dance hall ghosts
				{
					return _printReturn(currentMusic, "music");
				}
				case 0x24: // TODO: Is this just CVars?
				{
					// NOTE: Currently doesn't route output to a standardized method. Will need to be changed if output formatting changes.
					// TODO: Print CVar name
					int idx = *(s16*)currentLifePtr;
					currentLifePtr += 2;
					int temp = CVars[idx];
					appendFormatted("cvars[%i]:%i ", idx, temp);
					return(temp);
				}
				case 0x25: // STAGE
				{
					return _printReturn(actorPtr->stage, "stage");
				}
				case 0x26: // THROW
				{
					int objNum = *(s16*)currentLifePtr;
					currentLifePtr += 2;

					int retVal;
					if (ListWorldObjets[objNum].flags2 & 0x1000) {
						retVal = 1;
					} else {
						retVal = 0;
					}
					return _printFuncReturn(objNum, retVal, "throw");
				}
				default:
				{
					printf("Unhandled test type %X in evalVar\n", var1);
					FITD_throwFatal();
					assert(0); // Won't make it here, this just kills a compiler warning.
					return -1;
				}
			}
		}
	}
}

int evalVar2(const char* name)
{
	int var1;

	var1 = *(s16*)(currentLifePtr);
	currentLifePtr += 2;

	if (var1 == -1) {
		int temp = *(s16*)(currentLifePtr);
		currentLifePtr += 2;

		if (name)
			appendFormatted("%s:", name);
		appendFormatted("%d, ", temp);

		return(temp);
	} else
		if (var1 == 0) {
			int temp = *(s16*)(currentLifePtr);
			currentLifePtr += 2;

			if (name)
				appendFormatted("%s:", name);
			appendFormatted("vars[%d], ", temp);

			return(vars[temp]);
		} else {
			tObject* actorPtr = currentLifeActorPtr;
			int actorIdx = currentLifeActorIdx;

			if (var1 & 0x8000) {
				int objectNumber;

				objectNumber = *(s16*)currentLifePtr;

				actorIdx = ListWorldObjets[objectNumber].objIndex;

				currentLifePtr += 2;
				actorPtr = &objectTable[actorIdx];

				if (actorIdx == -1) {
					switch (var1 & 0x7FFF) {
						case 0x1F:
						{
							if (name)
								appendFormatted("%s:", name);
							appendFormatted("worldObjects[%d].room, ", objectNumber);

							return(ListWorldObjets[objectNumber].room);
							break;
						}
						case 0x24:
						{
							if (name)
								appendFormatted("%s:", name);
							appendFormatted("worldObjects[%d].stage, ", objectNumber);

							return(ListWorldObjets[objectNumber].stage);
							break;
						}
						default:
						{
							printf("Unsupported evalVar2 %X when actor not in room !\n", var1 & 0x7FFF);
							//FITD_throwFatal(); // assert(0);
							return false;
						}
					}
				}
			}
			{

				var1 &= 0x7FFF;

				var1--;

				switch (var1) {
					case 0x0:
					{
						int temp1 = actorPtr->COL[0];

						if (name)
							appendFormatted("%s:", name);
						appendFormatted("objectTable[%d].COL, ", temp1);

						if (temp1 != -1) {
							return(objectTable[temp1].indexInWorld);
						} else {
							return(-1);
						}
						break;
					}
					case 0x1:
					{
						if (name)
							appendFormatted("%s:", name);
						appendFormatted("HARD_DEC, ");
						return(actorPtr->HARD_DEC);
						break;
					}
					case 0x2:
					{
						if (name)
							appendFormatted("%s:", name);
						appendFormatted("HARD_COL, ");

						return(actorPtr->HARD_COL);
						break;
					}
					case 0x3:
					{
						if (name)
							appendFormatted("%s:", name);
						appendFormatted("HIT, ");
						int temp = actorPtr->HIT;

						if (temp == -1) {
							return(-1);
						} else {
							return(objectTable[temp].indexInWorld);
						}

						break;
					}
					case 0x4:
					{
						int temp = actorPtr->HIT_BY;

						if (temp == -1) {
							return(-1);
						} else {
							return(objectTable[temp].indexInWorld);
						}

						break;
					}
					case 0x5:
					{
						return(actorPtr->ANIM);
						break;
					}
					case 0x6:
					{
						return(actorPtr->END_ANIM);
						break;
					}
					case 0x7:
					{
						return(actorPtr->FRAME);
						break;
					}
					case 0x8:
					{
						return(actorPtr->END_FRAME);
						break;
					}
					case 0x9:
					{
						return(actorPtr->bodyNum);
						break;
					}
					case 0xA: // MARK
					{
						return(actorPtr->MARK);
						break;
					}
					case 0xB: // NUM_TRACK
					{
						return(actorPtr->trackNumber);
						break;
					}
					case 0xC: // CHRONO
					{
						return(evalChrono(&actorPtr->CHRONO) / 60); // recheck
						break;
					}
					case 0xD:
					{
						return(evalChrono(&actorPtr->ROOM_CHRONO) / 60); // recheck
						break;
					}
					case 0xE: // DIST
					{
						int worldObjectIdx = *(s16*)currentLifePtr;
						currentLifePtr += 2;

						int objectIdx = ListWorldObjets[worldObjectIdx].objIndex;

						int tempX;
						int tempY;
						int tempZ;

						if (objectIdx == -1) {
							if (ListWorldObjets[worldObjectIdx].room == currentRoom) {
								tempX = ListWorldObjets[worldObjectIdx].x;
								tempY = ListWorldObjets[worldObjectIdx].y;
								tempZ = ListWorldObjets[worldObjectIdx].z;
							} else {
								return(32000);
							}
						} else {
							tempX = objectTable[objectIdx].worldX;
							tempY = objectTable[objectIdx].worldY;
							tempZ = objectTable[objectIdx].worldZ;
						}

						return(calcDist(actorPtr->worldX, actorPtr->worldY, actorPtr->worldZ, tempX, tempY, tempZ));

						break;
					}
					case 0xF: // COL_BY
					{
						if (actorPtr->COL_BY == -1)
							return(-1);
						else
							return(objectTable[actorPtr->COL_BY].indexInWorld);
						break;
					}
					case 0x10: // found
					{
						if (ListWorldObjets[evalVar()].flags2 & 0x8000) {
							return(1);
						} else {
							return(0);
						}

						break;
					}
					case 0x11:
					{
						return action;
						break;
					}
					case 0x12: // POSREL
					{
						int objNum;

						objNum = *(s16*)currentLifePtr;
						currentLifePtr += 2;

						if (ListWorldObjets[objNum].objIndex == -1) {
							return 0;
						}

						return (getPosRel(actorPtr, &objectTable[ListWorldObjets[objNum].objIndex]));

						break;
					}
					case 0x13:
					{
						if (localJoyD & 4)
							return 4;
						if (localJoyD & 8)
							return 8;
						if (localJoyD & 1)
							return 1;
						if (localJoyD & 2)
							return 2;

						return 0;
						break;
					}
					case 0x14:
					{
						return(localClick);
						break;
					}
					case 0x15:
					{
						int temp1 = actorPtr->COL[0];
						if (temp1 == -1) {
							temp1 = actorPtr->COL_BY;
							if (temp1 == -1)
								return -1;
						}

						return objectTable[temp1].indexInWorld;
						break;
					}
					case 0x16:
					{
						return(actorPtr->alpha);
						break;
					}
					case 0x17:
					{
						return(actorPtr->beta);
						break;
					}
					case 0x18:
					{
						return(actorPtr->gamma);
						break;
					}
					case 0x19:
					{
						return(inHandTable[currentInventory]);
						break;
					}
					case 0x1A:
					{
						return(actorPtr->hitForce);
						break;
					}
					case 0x1B:
					{
						return(cameraDataTable[currentCamera] - &g_currentFloorCameraData[0]);
						break;
					}
					case 0x1C:
					{
						int temp = *(s16*)currentLifePtr;
						currentLifePtr += 2;
						return(rand() % temp);
						break;
					}
					case 0x1D:
					{
						return(actorPtr->falling);
						break;
					}
					case 0x1E:
					{
						return(actorPtr->room);
						break;
					}
					case 0x1F:
					{
						return(actorPtr->life);
						break;
					}
					case 0x20:
					{
						int objNum;

						objNum = *(s16*)currentLifePtr;
						currentLifePtr += 2;

						if (ListWorldObjets[objNum].flags2 & 0xC000) {
							return(1);
						} else {
							return(0);
						}

						break;
					}
					case 0x21: // TODO: music
					{
						return(currentMusic);
						break;
					}
					case 0x22: // c_var
					{
						int temp = CVars[*(s16*)currentLifePtr];
						currentLifePtr += 2;
						return(temp);
						break;
					}
					case 0x23:
					{
						return(actorPtr->stage);
						break;
					}
					case 0x24: // THROW
					{
						int objNum;

						objNum = *(s16*)currentLifePtr;
						currentLifePtr += 2;

						if (ListWorldObjets[objNum].flags2 & 0x1000) {
							return 1;
						} else {
							return 0;
						}
						break;
					}
					case 0x25: // get_matrix
					{
						int param1;
						int param2;

						param1 = *(s16*)currentLifePtr;
						currentLifePtr += 2;

						param2 = *(s16*)currentLifePtr;
						currentLifePtr += 2;

						return getMatrix(param1, actorIdx, ListWorldObjets[param2].objIndex);
						break;
					}
					case 0x26: // hard_mat
					{
						return actorPtr->hardMat;
						break;
					}
					case 0x27: // TEST_PROTECT
					{
						return 1;
						break;
					}
					case 0x2A: // related to samples
					{
						return 1;
						break;
					}
					default:
					{
						printf("Unhandled test type %X in evalVar\n", var1);
						FITD_throwFatal(); // assert(0);
						return 0;
						break;
					}
				}
			}
		}
}

