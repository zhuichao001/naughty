#include "Python.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define FILE_MODE 0X766


int shmem_open(const char *path, int map_size){
    int flags = O_RDWR | O_CREAT;
    int fd = shm_open(path, flags, FILE_MODE); 
    ftruncate(fd, map_size);
    return fd;
}

static PyObject *py_shm_open(PyObject *self, PyObject *args) {
	char *path;
	int map_size;
	if (!PyArg_ParseTuple(args, "si", &path, &map_size)) {
		return NULL;
	}
	int fd = shmem_open(path,map_size);
	return Py_BuildValue("i", fd);
}

/* Module method table */
static PyMethodDef methods[] = {
	{"shm_open",  py_shm_open, METH_VARARGS, "shmem open"},
	{ NULL, NULL, 0, NULL}
};

/* Module structure */
static struct PyModuleDef samplemodule = {
	PyModuleDef_HEAD_INIT,
	"shm_open",           /* name of module */
	"A sample module",  /* Doc string (may be NULL) */
	-1,                 /* Size of per-interpreter state or -1 */
	methods       /* Method table */
};

/* Module initialization function */
PyMODINIT_FUNC
PyInit_shmem(void) {
	  return PyModule_Create(&samplemodule);
}
