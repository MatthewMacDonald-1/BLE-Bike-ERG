
// For returning the pointer
typedef int (*logicDrawUpdateFuncPtr)(void);
// For calling the function
typedef logicDrawUpdateFuncPtr (*logicDrawUpdateFuncType)(void);

int DefaultLogicDrawFunc(void);

/**
 * Updates the draw and logic functions for the app. Useful for switching scenes and menus. Note: after calling this function that the newLogicUpdate function will be called before drawing the scene.
 * @param newLogicDrawUpdateFunc A pointer to the new logic Update and draw function.
 * @returns Returns the address of the replaced logicUpdateDrawFunc.
*/
logicDrawUpdateFuncPtr SwitchLogicAndDrawFunction(int (*newLogicDrawUpdateFunc)(void));

void SetVsyncStatus(bool val);
bool GetDebugStatus(void);
void DrawDebug(void);
bool GetBluetoothSupported(void);