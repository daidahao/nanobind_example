import numpy as np

import nanobind_example as m

def test_add():
    assert m.add(1, 2) == 3

def test_add_arrays():
    array1 = np.array([1, 2, 3], dtype=np.float64)
    array2 = np.array([4, 5, 6], dtype=np.float64)
    result = m.add_arrays(array1, array2)
    expected = np.array([5, 7, 9], dtype=np.float64)
    assert np.allclose(result, expected)
