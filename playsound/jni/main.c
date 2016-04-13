/*===============================================================================================
 PlaySound Example
 Copyright (c), Firelight Technologies Pty, Ltd 2004-2011.

 This example shows how to simply load and play multiple sounds.  This is about the simplest
 use of FMOD.
 This makes FMOD decode the into memory when it loads.  If the sounds are big and possibly take
 up a lot of ram, then it would be better to use the FMOD_CREATESTREAM flag so that it is
 streamed in realtime as it plays.
===============================================================================================*/

#include <jni.h>
#include <android/log.h>
#include <stdlib.h>
#include "fmod.h"
#include "fmod_errors.h"

#define NUM_SOUNDS 3

FMOD_SYSTEM  *gSystem  = 0;
FMOD_CHANNEL *gChannel = 0;
FMOD_SOUND	 *gSound[NUM_SOUNDS];

#define CHECK_RESULT(x) \
{ \
	FMOD_RESULT _result = x; \
	if (_result != FMOD_OK) \
	{ \
		__android_log_print(ANDROID_LOG_ERROR, "fmod", "FMOD error! (%d) %s\n%s:%d", _result, FMOD_ErrorString(_result), __FILE__, __LINE__); \
		exit(-1); \
	} \
}

void Java_com_goldrushcomputing_playsound_Example_cBegin(JNIEnv *env, jobject thiz)
{
	FMOD_RESULT result = FMOD_OK;

	result = FMOD_System_Create(&gSystem);
	CHECK_RESULT(result);

	result = FMOD_System_Init(gSystem, 32, FMOD_INIT_NORMAL, 0);
	CHECK_RESULT(result);

	//
	///sdcard/fmod/drumloop.wav
	result = FMOD_System_CreateSound(gSystem, "/sdcard/fmod/bosa.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &gSound[0]);
	CHECK_RESULT(result);

	///sdcard/fmod/jaguar.wav
	result = FMOD_System_CreateSound(gSystem, "/sdcard/fmod/bass.wav", FMOD_DEFAULT, 0, &gSound[1]);
	CHECK_RESULT(result);

	///sdcard/fmod/swish.wav
	result = FMOD_System_CreateSound(gSystem, "/sdcard/fmod/hat.wav", FMOD_DEFAULT, 0, &gSound[2]);
	CHECK_RESULT(result);
}


void Java_com_goldrushcomputing_playsound_Example_cBeginWith(JNIEnv *env,
		jobject thiz, jstring path1, jstring path2, jstring path3, jstring path4) {
	FMOD_RESULT result = FMOD_OK;

	result = FMOD_System_Create(&gSystem);
	CHECK_RESULT(result);

	int numBuffers = 0;
	int size = 0;
	result = FMOD_System_GetDSPBufferSize(gSystem, &size, &numBuffers);
	CHECK_RESULT(result);
	__android_log_print(ANDROID_LOG_ERROR, "fmod",
			"(Before)DSP buffer size is %d, num of buff is %d", size,
			numBuffers);

	result = FMOD_System_SetDSPBufferSize(gSystem, 256, numBuffers);
	CHECK_RESULT(result);
	__android_log_print(ANDROID_LOG_ERROR, "fmod", "Set DSP buffer size to %d",
			256);

	result = FMOD_System_GetDSPBufferSize(gSystem, &size, &numBuffers);
	CHECK_RESULT(result);
	__android_log_print(ANDROID_LOG_ERROR, "fmod",
			"(After)DSP buffer size is %d, num of buff is %d", size,
			numBuffers);

	result = FMOD_System_Init(gSystem, 32, FMOD_INIT_NORMAL, 0);
	CHECK_RESULT(result);

	const char *p1 = (*env)->GetStringUTFChars(env, path1, 0);
	const char *p2 = (*env)->GetStringUTFChars(env, path2, 0);
	const char *p3 = (*env)->GetStringUTFChars(env, path3, 0);
	const char *p4 = (*env)->GetStringUTFChars(env, path4, 0);



	result = FMOD_System_CreateSound(gSystem, p1,
			FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &gSound[0]);
	CHECK_RESULT(result);

	result = FMOD_System_CreateSound(gSystem, p2,
			FMOD_DEFAULT, 0, &gSound[1]);
	CHECK_RESULT(result);

	result = FMOD_System_CreateSound(gSystem, p3,
			FMOD_DEFAULT, 0, &gSound[2]);
	CHECK_RESULT(result);

	result = FMOD_System_CreateSound(gSystem, p4,
			FMOD_DEFAULT, 0, &gSound[3]);
	CHECK_RESULT(result);

}


void Java_com_goldrushcomputing_playsound_Example_cUpdate(JNIEnv *env, jobject thiz)
{
	FMOD_RESULT	result = FMOD_OK;

	result = FMOD_System_Update(gSystem);
	CHECK_RESULT(result);
}

void Java_com_goldrushcomputing_playsound_Example_cEnd(JNIEnv *env, jobject thiz)
{
	FMOD_RESULT result = FMOD_OK;
	unsigned int i = 0;

	for (i = 0; i < NUM_SOUNDS; i++)
	{
		result = FMOD_Sound_Release(gSound[i]);
		CHECK_RESULT(result);
	}

	result = FMOD_System_Release(gSystem);
	CHECK_RESULT(result);
}

void Java_com_goldrushcomputing_playsound_Example_cPlaySound(JNIEnv *env, jobject thiz, int id)
{
	FMOD_RESULT result = FMOD_OK;

	result = FMOD_System_PlaySound(gSystem, FMOD_CHANNEL_FREE, gSound[id], 0, &gChannel);
	CHECK_RESULT(result);
}

jint Java_com_goldrushcomputing_playsound_Example_cGetChannelsPlaying(JNIEnv *env, jobject thiz)
{
	FMOD_RESULT result = FMOD_OK;
	int chans = 0;

	result = FMOD_System_GetChannelsPlaying(gSystem, &chans);
	CHECK_RESULT(result);

	return chans;
}

jboolean Java_com_goldrushcomputing_playsound_Example_cGetPlaying(JNIEnv *env, jobject thiz)
{
	FMOD_RESULT result = FMOD_OK;
	FMOD_BOOL playing = 0;

	if (gChannel)
	{
		result = FMOD_Channel_IsPlaying(gChannel, &playing);
		if (result != FMOD_ERR_INVALID_HANDLE && result != FMOD_ERR_CHANNEL_STOLEN)
		{
			CHECK_RESULT(result);
		}
	}

	return playing;
}

jint Java_com_goldrushcomputing_playsound_Example_cGetPosition(JNIEnv *env, jobject thiz)
{
	FMOD_RESULT result = FMOD_OK;
	int position = 0;

	if (gChannel)
	{
		result = FMOD_Channel_GetPosition(gChannel, &position, FMOD_TIMEUNIT_MS);
		if (result != FMOD_ERR_INVALID_HANDLE && result != FMOD_ERR_CHANNEL_STOLEN)
		{
			CHECK_RESULT(result);
		}
	}

	return position;
}

jint Java_com_goldrushcomputing_playsound_Example_cGetLength(JNIEnv *env, jobject thiz)
{
	FMOD_RESULT result = FMOD_OK;
	FMOD_SOUND *sound = 0;
	int length = 0;

	if (gChannel)
	{
		result = FMOD_Channel_GetCurrentSound(gChannel, &sound);
		if (result != FMOD_ERR_INVALID_HANDLE && result != FMOD_ERR_CHANNEL_STOLEN)
		{
			CHECK_RESULT(result);
		}
	}

	if (sound)
	{
		result = FMOD_Sound_GetLength(sound, &length, FMOD_TIMEUNIT_MS);
		CHECK_RESULT(result);
	}

	return length;
}


