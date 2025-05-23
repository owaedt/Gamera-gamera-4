/*
 *
 * Copyright (C) 2001-2005 Ichiro Fujinaga, Michael Droettboom, and Karl MacMillan
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef GAMERACORE_INTERNAL
   #define GAMERACORE_INTERNAL
#endif
#include "gameramodule.hpp"

using namespace Gamera;

static PyTypeObject SizeType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

PyTypeObject* get_SizeType() {
  return &SizeType;
}

static PyObject* size_new(PyTypeObject* pytype, PyObject* args,
			  PyObject* kwds) {
  int width, height;
  if (PyArg_ParseTuple(args,  "ii:Size.__init__", &width, &height) <= 0)
    return 0;
  SizeObject* so;
  so = (SizeObject*)pytype->tp_alloc(pytype, 0);
  so->m_x = new Size((size_t)width, (size_t)height);
  return (PyObject*)so;
}

static void size_dealloc(PyObject* self) {
  SizeObject* x = (SizeObject*)self;
  delete x->m_x;
  self->ob_type->tp_free(self);
}

#define CREATE_GET_FUNC(name) static PyObject* size_get_##name(PyObject* self) {\
  Size* x = ((SizeObject*)self)->m_x; \
  return Py_BuildValue( "i", (int)x->name()); \
}

#define CREATE_SET_FUNC(name) static int size_set_##name(PyObject* self, PyObject* value) {\
  Size* x = ((SizeObject*)self)->m_x; \
  x->name((size_t)PyLong_AS_LONG(value)); \
  return 0; \
}

CREATE_GET_FUNC(width)
CREATE_SET_FUNC(width)
CREATE_GET_FUNC(height)
CREATE_SET_FUNC(height)

static PyObject* size_richcompare(PyObject* a, PyObject* b, int op) {
  if (!is_SizeObject(a) || !is_SizeObject(b)) {
    Py_XINCREF(Py_NotImplemented);
    return Py_NotImplemented;
  }

  Size& as = *((SizeObject*)a)->m_x;
  Size& bs = *((SizeObject*)b)->m_x;

  /*
    Only equality and inequality make sense.
  */
  bool cmp;
  switch (op) {
  case Py_LT:
    Py_XINCREF(Py_NotImplemented);
    return Py_NotImplemented;
  case Py_LE:
    Py_XINCREF(Py_NotImplemented);
    return Py_NotImplemented;
  case Py_EQ:
    cmp = as == bs;
    break;
  case Py_NE:
    cmp = as != bs;
    break;
  case Py_GT:
    Py_XINCREF(Py_NotImplemented);
    return Py_NotImplemented;
  case Py_GE:
    Py_XINCREF(Py_NotImplemented);
    return Py_NotImplemented;
  default:
    return 0; // cannot happen
  }
  if (cmp) {
    Py_XINCREF(Py_True);
    return Py_True;
  } else {
    Py_XINCREF(Py_False);
    return Py_False;
  }
}

static PyObject* size_repr(PyObject* self) {
  Size* x = ((SizeObject*)self)->m_x;
  return PyUnicode_FromFormat("Size(%i, %i)",
			     (int)x->width(), (int)x->height());
}

static PyGetSetDef size_getset[] = {
        { (char *)"width", (getter)size_get_width, (setter)size_set_width,
                (char *)"(int property)\n\nThe width of an object is the right boundary minus the left. This is the same as the number of columns *minus one.*", 0 },
        { (char *)"height", (getter)size_get_height, (setter)size_set_height,
                (char *)"(int property)\n\nThe height of an object is the lower boundary minus the upper boundary (remember that *y* is counted in the negative direction). This is the same as the number of rows *minus one*.", 0 },
        { nullptr }
};

void init_SizeType(PyObject* module_dict) {
  #ifdef Py_SET_TYPE
    Py_SET_TYPE(&SizeType, &PyType_Type);
  #else
    Py_TYPE(&SizeType) = &PyType_Type;
  #endif
  SizeType.tp_name =  "gameracore.Size";
  SizeType.tp_basicsize = sizeof(SizeObject);
  SizeType.tp_dealloc = size_dealloc;
  SizeType.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  SizeType.tp_getset = size_getset;
  SizeType.tp_new = size_new;
  SizeType.tp_getattro = PyObject_GenericGetAttr;
  SizeType.tp_alloc = nullptr; // PyType_GenericAlloc;
  SizeType.tp_richcompare = size_richcompare;
  SizeType.tp_free = nullptr; // _PyObject_Del;
  SizeType.tp_repr = size_repr;
  SizeType.tp_doc =  "Size stores a size (*width*, *height*). This is almost the same as ``Dim``, but stores the number of columns/rows minus one:\n\n  *width* == *ncols* - 1\n\n  *height* == *nrows* - 1";
  PyType_Ready(&SizeType);
  PyDict_SetItemString(module_dict, "Size", (PyObject*)&SizeType);
}
