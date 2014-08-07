#include <unix.h>
#include <pkcs11.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "testlib.h"

int va_counter;
int fc_counter;

enum {
	ROBOT_NONE,
	ROBOT_AUTO,
	ROBOT_MECHANICAL_TURK,
} robot_type;

void verify_null(CK_UTF8CHAR* string, size_t length, int expect, char* msg) {
	int nullCount = 0;
	char* buf = malloc(length + 1);
	int i;
	for(i=0; i<length; i++) {
		if(string[i] == '\0') {
			nullCount++;
		}
	}
	verbose_assert(nullCount == expect);

	strncpy(buf, string, length + 1);
	buf[length] = '\0';
	printf(msg, buf);
}

CK_BBOOL have_robot() {
	char* envvar = getenv("EID_ROBOT_STYLE");
	if(envvar == NULL) {
		robot_type = ROBOT_NONE;
		return CK_FALSE;
	}
	if(!strcmp(envvar, "auto")) {
		robot_type = ROBOT_AUTO;
		return CK_TRUE;
	}
	if(!strcmp(envvar, "manual")) {
		robot_type = ROBOT_MECHANICAL_TURK;
		return CK_TRUE;
	}
}

typedef struct {
	const char* rvname;
	int result;
} ckrvdecode;

#define ADD_CKRV(ckrv, defaultrv) decodes[ckrv].rvname = #ckrv; decodes[ckrv].result = defaultrv

int ckrv_decode(CK_RV rv, int count, ...) {
	va_list ap;
	ckrvdecode *decodes = calloc(CKR_FUNCTION_REJECTED + 1, sizeof(ckrvdecode));
	int i;

	printf("function %d: ", fc_counter++);

	ADD_CKRV(CKR_ARGUMENTS_BAD, TEST_RV_FAIL);
	ADD_CKRV(CKR_BUFFER_TOO_SMALL, TEST_RV_FAIL);
	ADD_CKRV(CKR_CANT_LOCK, TEST_RV_FAIL);
	ADD_CKRV(CKR_CRYPTOKI_ALREADY_INITIALIZED, TEST_RV_FAIL);
	ADD_CKRV(CKR_CRYPTOKI_NOT_INITIALIZED, TEST_RV_FAIL);
	ADD_CKRV(CKR_DEVICE_ERROR, TEST_RV_FAIL);
	ADD_CKRV(CKR_DEVICE_MEMORY, TEST_RV_FAIL);
	ADD_CKRV(CKR_DEVICE_REMOVED, TEST_RV_FAIL);
	ADD_CKRV(CKR_FUNCTION_FAILED, TEST_RV_FAIL);
	ADD_CKRV(CKR_FUNCTION_NOT_SUPPORTED, TEST_RV_SKIP);
	ADD_CKRV(CKR_GENERAL_ERROR, TEST_RV_FAIL);
	ADD_CKRV(CKR_HOST_MEMORY, TEST_RV_FAIL);
	ADD_CKRV(CKR_MECHANISM_INVALID, TEST_RV_FAIL);
	ADD_CKRV(CKR_NEED_TO_CREATE_THREADS, TEST_RV_FAIL);
	ADD_CKRV(CKR_NO_EVENT, TEST_RV_FAIL);
	ADD_CKRV(CKR_OK, TEST_RV_OK);
	ADD_CKRV(CKR_SLOT_ID_INVALID, TEST_RV_FAIL);
	ADD_CKRV(CKR_TOKEN_NOT_PRESENT, TEST_RV_FAIL);
	ADD_CKRV(CKR_TOKEN_NOT_RECOGNIZED, TEST_RV_FAIL);

	va_start(ap, count);
	for(i=0; i<count; i++) {
		CK_RV modrv = va_arg(ap, CK_RV);
		int toreturn = va_arg(ap, int);
		assert(modrv <= CKR_FUNCTION_REJECTED);
		assert(decodes[modrv].rvname != NULL);
		decodes[modrv].result = toreturn;
	}

	if(decodes[rv].rvname != NULL) {
		printf("%s\n", decodes[rv].rvname);
		return decodes[rv].result;
	} else {
		printf("unknown CK_RV 0x%08x", rv);
		return TEST_RV_FAIL;
	}
}

void robot_insert_card() {
	char c;
	switch(robot_type) {
		case ROBOT_NONE:
			fprintf(stderr, "E: robot needed, no robot configured\n");
			exit(EXIT_FAILURE);
		case ROBOT_AUTO:
			fprintf(stderr, "Auto robot not yet implemented\n");
			exit(TEST_RV_SKIP);
		case ROBOT_MECHANICAL_TURK:
			printf("Please insert a card and press <enter>\n");
			scanf("%c", &c);
	}
}

void robot_remove_card() {
	char c;
	switch(robot_type) {
		case ROBOT_NONE:
			fprintf(stderr, "E: robot needed, no robot configured\n");
			exit(EXIT_FAILURE);
		case ROBOT_AUTO:
			fprintf(stderr, "Auto robot not yet implemented\n");
			exit(TEST_RV_SKIP);
		case ROBOT_MECHANICAL_TURK:
			printf("Please remove the card from the slot and press <enter>\n");
			scanf("%c", &c);
	}
}

void robot_insert_reader() {
}

void robot_remove_reader() {
}

int init_tests() {
	int count = 0;
#if (TESTS_TO_RUN & TEST_INIT_FINALIZE)
	test_ptrs[0] = init_finalize;
	count++;
#endif
#if (TESTS_TO_RUN & TEST_GETINFO)
	test_ptrs[1] = getinfo;
	count++;
#endif
#if (TESTS_TO_RUN & TEST_FUNCLIST)
	test_ptrs[2] = funclist;
	count++;
#endif
#if (TESTS_TO_RUN & TEST_SLOTLIST)
	test_ptrs[3] = slotlist;
	count++;
#endif
#if (TESTS_TO_RUN & TEST_SLOTINFO)
	test_ptrs[4] = slotinfo;
	count++;
#endif
#if (TESTS_TO_RUN & TEST_TKINFO)
	test_ptrs[5] = tkinfo;
	count++;
#endif
#if (TESTS_TO_RUN & TEST_DOUBLE_INIT)
	test_ptrs[6] = double_init;
	count++;
#endif
#if (TESTS_TO_RUN & TEST_FORK_INIT)
	test_ptrs[7] = fork_init;
	count++;
#endif
#if (TESTS_TO_RUN & TEST_SLOTEVENT)
	test_ptrs[8] = slotevent;
	count++;
#endif
#if (TESTS_TO_RUN & TEST_MECHLIST)
	test_ptrs[9] = mechlist;
	count++;
#endif
#if (TESTS_TO_RUN & TEST_MECHINFO)
	test_ptrs[10] = mechinfo;
	count++;
#endif
	verbose_assert(count > 0);
	if(count==1) {
		return 1;
	} else {
		return 0;
	}
}