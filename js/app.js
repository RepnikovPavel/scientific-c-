_BoardsContainer = document.getElementById('PlottingViewer');
_Gallary = document.getElementById('Gallary');
//_DrawingWrapper = document.getElementById('DrawingWrapper');

//here state of all plottings - meta,data etc.
var _current_drawing;
var _is_some_drawing_selected = 0;
var _state_of_all_drawings = {} // key-  ;value- params for plotting,meta info
var _drawings_count = 0;
//

var _z_spacing  = -1000;
var _BoardsArr  = [];
var _z_Array = [];
var _boards_count = 0;
var _screen_z_pos = 0;
var _current_position_of_the_reference_element = _screen_z_pos;
var _current_board;

var _num_of_rows_in_BoardPage = 2;
var _num_of_columns_in_BoardPage = 3;

var _BoardsNames = {} // key- board id that user choose, value - _board_count that is hidden id(board index)
var _NumOfPagesInBoard = {} // key - board index , value - num of pages in this board
var _NumOfDrawingsInBoard = {} // key- hidden board id, value- num of Drawings in that board.
var _ChildsOfBoard = {}
var _ChildsOfBoard_x_pos = {}
var _current_posistion_of_the_reference_page = {} // key- board index, value - current pos
var _screen_x_pos = 0;
var _x_spacing =1920;
var _current_page = {}//key-board_index , value - index of current page on this board

var _screen_width = window.screen.width;
var _screen_height = window.screen.height;


var _y_spasing = 1080;

function Draw2D(Draw_Field, draw_data,draw_layout,draw_config) {

    Plotly.newPlot(Draw_Field, draw_data, draw_layout, draw_config);
};
function GetPostionOfLastBoard() {
    if (_boards_count > 1) {
        return _current_position_of_the_reference_element + (_boards_count-1) * _z_spacing;
    }
    else {
        return _current_position_of_the_reference_element;
    }
}
function GetPositionOfFirstBoard() {
    return _current_position_of_the_reference_element;
}

function GetPositionOfLastPageInBoardByIndex(board_index)
{
    var last_index = _NumOfPagesInBoard[board_index];
    return _screen_x_pos + _current_posistion_of_the_reference_page[board_index] + (last_index - 1) * _x_spacing;
}
function GetPositionOfFirstPageByIndex(board_index) {
    return _current_posistion_of_the_reference_page[board_index];
}

function SetPositionForCreatedNowBoard(board_index, board_obj) {
    var z_pos = _screen_z_pos + _current_position_of_the_reference_element + board_index * _z_spacing; 
    var transform = `translateZ(${z_pos}px)`;
    var opacity = 0;
    if (z_pos < _screen_z_pos) {
        opacity = 0;
    }
    else {
        opacity = 1;
    }
    board_obj.setAttribute('style', `transform: ${transform}; opacity: ${opacity}`);
}

function SetPositionForCreatedNowPage(board_index, page_obj, page_index) {
    var x_pos = _screen_x_pos + _current_posistion_of_the_reference_page[board_index] + page_index * _x_spacing;
    var transform = `translateX(${x_pos}px)`;
    var opacity = 0;
    if (x_pos > _screen_x_pos) {
        opacity = 0;
    }
    else {
        opacity = 1;
    }

    page_obj.setAttribute('style', `transform: ${transform}; opacity: ${opacity}`);
}

function SetDisplayPropertyForCreatedNowBoard(board_obj, board_index) {
    if (_current_board < board_index) {
        board_obj.setAttribute('style', 'display:none;');
    }
    else
    {
        board_obj.setAttribute('style', 'display:block;');
    }
}

var _AlignedPage;

document.body.addEventListener('keypress', function (event) {
    if (event.code == 'Digit1') {
        if (_is_some_drawing_selected==0) {
            if (_screen_z_pos > GetPostionOfLastBoard()) {
                _BoardsArr.forEach(function (el, el_index) {
                    _z_Array[el_index] -= _z_spacing;
                    var _Board = _BoardsArr[el_index];
                    var _transform = `translateZ(${_z_Array[el_index]}px)`;
                    var _opacity = 0;
                    if (_z_Array[el_index] < _screen_z_pos) {
                        _opacity = 0;
                    }
                    else {
                        _opacity = _z_Array[el_index] < (Math.abs(_z_spacing) / 1.5) ? 1 : 0;
                    }
                    var disable_element = _opacity == 0 ? 'none' : 'block';
                    _Board.setAttribute('style', `transform: ${_transform}; opacity: ${_opacity};display:${disable_element}`);


                });
                _current_position_of_the_reference_element -= _z_spacing;
                _current_board += 1;

            }
        }

    }

    if (event.code == 'Digit2') {

        if (_is_some_drawing_selected == 0) {
            if (_screen_z_pos < GetPositionOfFirstBoard()) {
                _BoardsArr.forEach(function (el, el_index) {
                    _z_Array[el_index] += _z_spacing;
                    var _Board = _BoardsArr[el_index];
                    var _transform = `translateZ(${_z_Array[el_index]}px)`;
                    var _opacity = 0;
                    if (_z_Array[el_index] < _screen_z_pos) {
                        _opacity = 0;
                    }
                    else {
                        _opacity = _z_Array[el_index] < (Math.abs(_z_spacing) / 1.5) ? 1 : 0;
                    }
                    var disable_element = _opacity == 0 ? 'none' : 'block';
                    _Board.setAttribute('style', `transform: ${_transform}; opacity: ${_opacity};display:${disable_element}`);

                });
                _current_position_of_the_reference_element += _z_spacing;
                _current_board -= 1;

            }
        }


    }

    if (event.code == 'Digit3') {

        if (_is_some_drawing_selected == 0) {
            //user go to left
            if (_screen_x_pos > GetPositionOfFirstPageByIndex(_current_board)) {
                _ChildsOfBoard[_current_board].forEach(function (el, el_index) {

                    _ChildsOfBoard_x_pos[_current_board][el_index] += _x_spacing;
                    opacity = 1;
                    if (_ChildsOfBoard_x_pos[_current_board][el_index] != _screen_x_pos) {
                        opacity = 0;
                    }

                    var transform = `translateX(${_ChildsOfBoard_x_pos[_current_board][el_index]}px)`;
                    el.setAttribute('style', `transform: ${transform}; opacity: ${opacity}`);

                });
                _current_posistion_of_the_reference_page[_current_board] += _x_spacing;
                _current_page[_current_board] -= 1;
            }
        }

    }
    if (event.code == 'Digit4') {

        if (_is_some_drawing_selected == 0) {
            //user go to right
            if (_screen_x_pos < GetPositionOfLastPageInBoardByIndex(_current_board)) {
                _ChildsOfBoard[_current_board].forEach(function (el, el_index) {

                    _ChildsOfBoard_x_pos[_current_board][el_index] -= _x_spacing;
                    opacity = 1;
                    if (_ChildsOfBoard_x_pos[_current_board][el_index] != _screen_x_pos) {
                        opacity = 0;
                    }
                    var transform = `translateX(${_ChildsOfBoard_x_pos[_current_board][el_index]}px)`;
                    el.setAttribute('style', `transform: ${transform}; opacity: ${opacity}`);

                });
                _current_posistion_of_the_reference_page[_current_board] -= _x_spacing;
                _current_page[_current_board] += 1;
            }
        }

    }

    if (event.code == 'KeyZ') {
        if (_is_some_drawing_selected == 1) {
            var DrawingWrapper = document.getElementById('wrapper_for_drawing_full_screen_mode');
            DrawingWrapper.remove();
            var transform_for_page = `translateY(${0}px)`;
            _AlignedPage.setAttribute('style', `transform: ${transform_for_page};`);
            _is_some_drawing_selected = 0;

        }
    }

});


var _plwr_current_board = 0;

function CreateNewDrawing(board_index, page_obj, draw_data, draw_layout, draw_config) {
    _NumOfDrawingsInBoard[board_index] += 1;
    let _new_drawing = document.createElement("div");
    _new_drawing.className = 'DrawingClass';
    _new_drawing.id = 'Board_' + board_index + '_Page_' + (_NumOfPagesInBoard[board_index] - 1).toString() + '_Drawing_' + (_NumOfDrawingsInBoard[board_index] - 1).toString();
    page_obj.appendChild(_new_drawing);
    Draw2D(_new_drawing, draw_data, draw_layout, draw_config);


    _new_drawing.onclick = function () {
        _is_some_drawing_selected = 1;

        if (_current_drawing) {
            _current_drawing.parentNode.removeChild(_current_drawing);
        }


        x_pos = _ChildsOfBoard_x_pos[_current_page[_current_board]];

        // create new tmp page. move current page.  
        var new_tmp_page = document.createElement("div");
        _BoardsArr[board_index].appendChild(new_tmp_page);
        new_tmp_page.className = 'DrawingWrapperClass';
        new_tmp_page.id = 'wrapper_for_drawing_full_screen_mode';

        var new_tmp_drawing = document.createElement("div");
        new_tmp_drawing.className = 'DrawingInFullScreenClass';
        new_tmp_drawing.id = 'drawing_full_screen';
                
        new_tmp_page.appendChild(new_tmp_drawing);

        var transform_for_page = `translateY(${_y_spasing}px)`;
        page_obj.setAttribute('style', `transform: ${transform_for_page};`);
        var transform_for_tmp = `translateX(${x_pos}px), translateY(${0}px)`;
        new_tmp_page.setAttribute('style', `transform: ${transform_for_tmp};`);

        _AlignedPage = page_obj;
        Draw2D(new_tmp_drawing, draw_data, draw_layout, draw_config);



    }
}




var ws = null;
/*connect.onclick = function () {*/
    //ws = new WebSocket(uri.value);
ws = new WebSocket("ws://127.0.0.1:8080");
ws.onopen = function (ev) {
    console.log("[connection opened]");
};
ws.onclose = function (ev) {
    console.log("[connection closed]");
};
ws.onmessage = function (ev) {

    var _text = ev.data;
    var _data = JSON.parse(_text);


    var _my_args = _data.my_args;
    var _board_id = _my_args.board_id;

    var _draw_data = _data.plotly_data;
    var _draw_layout = _data.plotly_layout;
    var _draw_config = _data.plotly_config;

    if (_board_id in _BoardsNames) {
        if (_NumOfDrawingsInBoard[_this_board_index] % (_num_of_columns_in_BoardPage * _num_of_rows_in_BoardPage) == 0) {
            _this_board_index = _BoardsNames[_board_id];
            // create new page on board
            _NumOfPagesInBoard[_this_board_index] += 1;
            let _new_board_page = document.createElement("div");
            _new_board_page.className = 'BoardPageClass';
            _new_board_page.id = 'Board_' + _this_board_index + '_Page_' + (_NumOfPagesInBoard[_this_board_index] - 1).toString();
            _BoardsArr[_this_board_index].appendChild(_new_board_page);
            _ChildsOfBoard[_this_board_index].push(_new_board_page);
            _ChildsOfBoard_x_pos[_this_board_index].push(_screen_x_pos + _current_posistion_of_the_reference_page[_this_board_index] + (_NumOfPagesInBoard[_this_board_index] - 1) * _x_spacing);
            SetPositionForCreatedNowPage(_this_board_index, _new_board_page, _NumOfPagesInBoard[_this_board_index] - 1);
            
        }

        //create new drawing in the last page on board
        CreateNewDrawing(_this_board_index, _ChildsOfBoard[_this_board_index][_NumOfPagesInBoard[_this_board_index] - 1], _draw_data, _draw_layout, _draw_config);

    }
    else
    {
        //here board initialization

        //create new board with _board_id
        _BoardsNames[_board_id] = _boards_count;

        _boards_count += 1;
        _this_board_index = _BoardsNames[_board_id];
        if (_boards_count == 1) {
            _current_board = _BoardsNames[_board_id];
        }

        let _new_board = document.createElement("div");
        _new_board.className = 'BoardClass';
        _new_board.id = 'Board_' + _this_board_index;
        _Gallary.appendChild(_new_board);
        _NumOfDrawingsInBoard[_this_board_index] = 0;
        _NumOfPagesInBoard[_this_board_index] = 0;
        _BoardsArr.push(_new_board);
        _z_Array.push(_screen_z_pos + _current_position_of_the_reference_element + (_boards_count-1) * _z_spacing);
        _ChildsOfBoard[_this_board_index] = [];
        _ChildsOfBoard_x_pos[_this_board_index] = [];
        _current_posistion_of_the_reference_page[_this_board_index] = _screen_x_pos;
        SetPositionForCreatedNowBoard(_this_board_index, _new_board);
        SetDisplayPropertyForCreatedNowBoard(_new_board, _this_board_index);
        //create first board page 
        _NumOfPagesInBoard[_this_board_index] += 1;
        let _new_board_page = document.createElement("div");
        _new_board_page.className = 'BoardPageClass';
        _new_board_page.id = 'Board_' + _this_board_index + '_Page_' + (_NumOfPagesInBoard[_this_board_index] - 1).toString();
        _new_board.appendChild(_new_board_page);
        _ChildsOfBoard[_this_board_index].push(_new_board_page);
        _ChildsOfBoard_x_pos[_this_board_index].push(_screen_x_pos + _current_posistion_of_the_reference_page[_this_board_index] + (_NumOfPagesInBoard[_this_board_index] - 1) * _x_spacing);
        SetPositionForCreatedNowPage(_this_board_index, _new_board_page, _NumOfPagesInBoard[_this_board_index] - 1);
        _current_page[_this_board_index] = 0;

        //create new drawing in first page on new board
        CreateNewDrawing(_this_board_index, _new_board_page, _draw_data, _draw_layout, _draw_config);
    }

};
ws.onerror = function (ev) {
    console.log("[error]");
    console.log(ev);
};
