#include <vector>

extern "C" {
#include <Python.h>
}

namespace compare_fragments
{
	typedef std::vector<double>  channel_t;
	typedef std::vector<channel_t> row_t;
	typedef std::vector<row_t>   image_t;

	static double diff(const image_t &a, const image_t &b)
	{
		double difference = 0;
		// Counting MSE 
		for (size_t y = 0; y < a.size(); ++y) {
			for (size_t x = 0; x < b.size(); ++x) {
				for (size_t ch = 0; ch < 3; ++ch)
				{
					difference += double((int(a[y][x][ch]) - int(b[y][x][ch])) * (int(a[y][x][ch]) - int(b[y][x][ch])));
				}
			}
		}
		return difference;
	}
}


static compare_fragments::image_t pyobject_to_cxx(PyObject * py_matrix)
{
	compare_fragments::image_t result;
	result.resize(PyObject_Length(py_matrix));
	for (size_t i = 0; i < result.size(); ++i) {
		PyObject * py_row = PyList_GetItem(py_matrix, i);
		compare_fragments::row_t & row = result[i];
		row.resize(PyObject_Length(py_row));

		for (size_t j = 0; j<row.size(); ++j) {
			PyObject * py_channel = PyList_GetItem(py_row, j);
			compare_fragments::channel_t & channel = row[j];
			channel.resize(PyObject_Length(py_channel));

			for (size_t c = 0; c < 3; ++c)
			{
				PyObject * py_elem = PyList_GetItem(py_channel, c);
				const double elem = PyFloat_AsDouble(py_elem);
				channel[c] = elem;
			}

		}

	}
	return result;
}

static PyObject * cxx_to_pyobject(const compare_fragments::image_t &matrix)
{
	PyObject * result = PyList_New(matrix.size());
	for (size_t i = 0; i < matrix.size(); ++i) {
		const compare_fragments::row_t & row = matrix[i];
		PyObject * py_row = PyList_New(row.size());
		PyList_SetItem(result, i, py_row);

		for (size_t j = 0; j<row.size(); ++j) {
			const compare_fragments::channel_t & channel = row[j];
			PyObject * py_channel = PyList_New(channel.size());
			PyList_SetItem(py_row, j, py_channel);

			for (size_t c = 0; c < channel.size(); ++c)
			{
				const double elem = channel[c];
				PyObject * py_elem = PyFloat_FromDouble(elem);
				PyList_SetItem(py_channel, c, py_elem);
			}

		}
	}
	return result;
}

static PyObject * diff_frags(PyObject * module, PyObject * args)
{	
	
	PyObject * py_a = PyTuple_GetItem(args, 0);
	PyObject * py_b = PyTuple_GetItem(args, 1);

	
	const compare_fragments::image_t a = pyobject_to_cxx(py_a);
	const compare_fragments::image_t b = pyobject_to_cxx(py_b);
	

	double result = compare_fragments::diff(a, b);
	return PyFloat_FromDouble(result);
	//return PyFloat_FromDouble(123);
		
}

PyMODINIT_FUNC PyInit_compareimg()
{
	static PyMethodDef ModuleMethods[] = {
		{ "diff_frag", diff_frags, METH_VARARGS, "Compare image" },
		{ NULL, NULL, 0, NULL }
	};
	static PyModuleDef ModuleDef = {
		PyModuleDef_HEAD_INIT,
		"compareimg",
		"Compare image",
		-1, ModuleMethods,
		NULL, NULL, NULL, NULL
	};
	PyObject * module = PyModule_Create(&ModuleDef);
	return module;
}