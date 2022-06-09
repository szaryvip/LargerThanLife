from src.gui import GUI
import pytest


def test_check_conditions():
    gui = GUI()
    correct = [2, 3, 4, 7]
    assert gui.check_condition("2-4,7") == correct


def test_check_conditions_error():
    with pytest.raises(ValueError):
        gui = GUI()
        gui.check_condition('2-3,gh')


def test_check_conditions_empty():
    gui = GUI()
    with pytest.raises(ValueError):
        assert gui.check_condition("") == []


def test_close_gui_change():
    gui = GUI()
    assert gui._close_gui is False
    gui.set_gui_close()
    assert gui._close_gui is True


def test_if_close():
    gui = GUI()
    gui.set_gui_close()
    assert gui.if_close() is True


def test_set_close_after_set():
    gui = GUI()
    gui.set_gui_close()
    assert gui.if_close() is True
    gui.set_gui_close()
    assert gui.if_close() is True


def test_get_params_empty():
    gui = GUI()
    assert gui.get_params() == {}


def test_get_params_from_json():
    gui = GUI()
    assert gui._params == {}
    gui.get_params_from_json("tests/export_rules.json")
    assert gui._params["Rr"] == 1
    assert gui._params["Bb"] == [2, 3]
    assert gui._params["Nn"] == 'm'


def test_get_params_not_empty():
    gui = GUI()
    gui.get_params_from_json("tests/export_rules.json")
    assert gui._params["Rr"] == 1
    assert gui._params["Bb"] == [2, 3]
    assert gui._params["Nn"] == 'm'
