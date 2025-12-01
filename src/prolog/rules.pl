% load library to read lines from files
:- use_module(library(readutil)).

% load list utilities for nth0 and maplist
:- use_module(library(lists)).

% store the path to the board state file
state_file('data/current_state.txt').

% store the path to the move query file
query_file('data/move_query.txt').

% store the path to the response file
response_file('data/move_response.txt').

% reads the board file and extracts Board, Size, and CurrentPlayer
read_board_state(Board, Size, CurrentPlayer) :-
    % get the path to the state file
    state_file(Path),
    % safely open and close the file while reading content
    setup_call_cleanup(
        open(Path, read, Stream),
        % read structured data from the file
        read_board_stream(Stream, Board, Size, CurrentPlayer),
        close(Stream)
    ).

% reads the board size, N rows, and current player from a stream
read_board_stream(Stream, Board, Size, CurrentPlayer) :-
    % read first line as codes for board size
    read_line_to_codes(Stream, SizeCodes),
    % convert codes to number for board size
    number_codes(Size, SizeCodes),
    % read next Size lines as board rows
    read_board_rows(Stream, Size, Board),
    % read the final line as player number codes
    read_line_to_codes(Stream, PlayerCodes),
    % convert to numeric player value
    number_codes(CurrentPlayer, PlayerCodes).

% base case: no more rows to read when N = 0
read_board_rows(_, 0, []) :- !.

% recursively read N rows from the stream
read_board_rows(Stream, N, [Row|Rest]) :-
    % read one line of CSV codes
    read_line_to_codes(Stream, LineCodes),
    % convert CSV codes to list of numbers
    split_codes_to_numbers(LineCodes, Row),
    % decrement remaining rows
    N1 is N - 1,
    % recursively read remaining rows
    read_board_rows(Stream, N1, Rest).

% converts character codes for a CSV line into actual numbers
split_codes_to_numbers(Codes, Numbers) :-
    % convert codes to string
    string_codes(Str, Codes),
    % split by commas ignoring whitespace
    split_string(Str, ",", " \t\r\n", Parts),
    % convert each substring into a number
    maplist(number_string, Numbers, Parts).

% reads Row, Col, and Player from the move query file
read_move_query(Row, Col, Player) :-
    % get query file path
    query_file(Path),
    % safely open and close while reading
    setup_call_cleanup(
        open(Path, read, Stream),
        (
            % read one line of input codes
            read_line_to_codes(Stream, LineCodes),
            % convert into a Prolog string
            string_codes(Line, LineCodes),
            % split into row, col, player substrings
            split_string(Line, ",", " \t\r\n", [RowStr, ColStr, PlayerStr]),
            % convert each substring into numbers
            number_string(Row, RowStr),
            number_string(Col, ColStr),
            number_string(Player, PlayerStr)
        ),
        close(Stream)
    ).

% checks that Index is within the board range 0 .. Size-1
valid_position(Index, Size) :-
    % lower bound check
    Index >= 0,
    % upper bound check
    Index < Size.

% checks if a specific board cell contains a zero
is_empty(Board, Row, Col) :-
    % extract row at index Row
    nth0(Row, Board, BoardRow),
    % extract cell value at col and ensure it is zero
    nth0(Col, BoardRow, 0).

% valid players are 1 (human) and 2 (AI)
valid_player(1).
valid_player(2).

% checks whether a move is legal in terms of bounds, player, and emptiness
valid_move(Board, Row, Col, Player) :-
    % compute board size
    length(Board, Size),
    % verify row is in bounds
    valid_position(Row, Size),
    % verify col is in bounds
    valid_position(Col, Size),
    % verify the player number is valid
    valid_player(Player),
    % verify the target cell is empty
    is_empty(Board, Row, Col).

% succeeds when List has all same non-zero value and binds Value
all_same_nonzero([Value|Rest], Value) :-
    % ensure leading element is not zero
    Value \= 0,
    % ensure entire list matches Value
    maplist(=(Value), Rest).

% checks each row of the board to see if it contains a winning line
row_winner(Board, Winner) :-
    % pick one row from the board
    member(Row, Board),
    % test if row has all same non-zero values
    all_same_nonzero(Row, Winner).

% collects all values in column ColIndex into Values list
column_values(Board, ColIndex, Values) :-
    % gather Values from each row at given column
    findall(Value,
            ( nth0(_, Board, Row),
              nth0(ColIndex, Row, Value)
            ),
            Values).

% checks all columns to find a winning column
column_winner(Board, Winner) :-
    % compute board size
    length(Board, Size),
    % iterate over all column indices
    between(0, Size - 1, ColIndex),
    % collect column values
    column_values(Board, ColIndex, Values),
    % test if column forms a win
    all_same_nonzero(Values, Winner).

% extracts main diagonal values from board
diag_values(Board, Values) :-
    % collect diagonal elements where row index = col index
    findall(Value,
            ( nth0(Index, Board, Row),
              nth0(Index, Row, Value)
            ),
            Values).

% extracts anti-diagonal values from board
anti_diag_values(Board, Values) :-
    % compute board size
    length(Board, Size),
    % compute largest index
    MaxIndex is Size - 1,
    % pick value at row i, col (MaxIndex - i)
    findall(Value,
            ( nth0(Index, Board, Row),
              ColIndex is MaxIndex - Index,
              nth0(ColIndex, Row, Value)
            ),
            Values).

% checks rows, columns, and diagonals for a winner
check_winner(Board, Winner) :-
    % check any row win
    row_winner(Board, Winner)
    % check any column win
    ; column_winner(Board, Winner)
    % check main diagonal win
    ; diag_values(Board, Diag),
      all_same_nonzero(Diag, Winner)
    % check anti-diagonal win
    ; anti_diag_values(Board, AntiDiag),
      all_same_nonzero(AntiDiag, Winner).

% true when the board contains no zero values
board_full(Board) :-
    % ensure no row contains a zero anywhere
    \+ (member(Row, Board), member(0, Row)).

% true when the board has a winner or is full
game_over(Board) :-
    % any win ends the game
    check_winner(Board, _)
    % full board also ends game
    ; board_full(Board).

% writes "valid" or "invalid" into the response file
write_response(Response) :-
    % get file path
    response_file(Path),
    % safely open and close the output stream
    setup_call_cleanup(
        open(Path, write, Stream),
        (
            % write the outcome atom
            write(Stream, Response),
            % write newline terminator
            nl(Stream)
        ),
        close(Stream)
    ).

% validates a move by comparing player turn and board rules
validate_move :-
    % catch any errors so C++ receives a response
    catch(
        (
            % load board state
            read_board_state(Board, _Size, CurrentPlayer),
            % load move query
            read_move_query(Row, Col, Player),
            % verify player turn and legality
            (   Player =:= CurrentPlayer,
                valid_move(Board, Row, Col, Player)
            % if valid, write "valid"
            ->  write_response(valid)
            % otherwise write "invalid"
            ;   write_response(invalid)
            )
        ),
        Error,
        (
            % print error for debugging
            print_message(error, Error),
            % respond with invalid on error
            write_response(invalid)
        )
    ),
    % cut to prevent backtracking
    !.

% run validate_move automatically when called from command line
:- initialization(main).

% main entry point for SWI-Prolog execution
main :-
    % run validate_move but ignore success/failure result
    ( validate_move -> true ; true ),
    % halt Prolog runtime
    halt.