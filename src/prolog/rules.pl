/* rules.pl
 * Game rules and move validation in Prolog
 *
 * Prolog acts as the rules referee. It reads the board state written by C++,
 * performs legality checks, and then returns either "valid" or "invalid" to
 * the C++ driver. The predicates below mirror the file formats documented in
 * data/README.md so future maintainers can cross-reference quickly.
 */

/*
 * Import a few helper libraries: readutil for reading entire lines and lists
 * for convenient nth0/3 accessors.
 */
:- use_module(library(readutil)).
:- use_module(library(lists)).

/*
 * Centralize the filenames so we only change them in one place if necessary.
 */
state_file('data/current_state.txt').
query_file('data/move_query.txt').
response_file('data/move_response.txt').

/*
 * read_board_state(-Board, -Size, -CurrentPlayer)
 * Reads the state file and returns the parsed board as a list of lists along
 * with the size and the player whose turn it is.
 */
read_board_state(Board, Size, CurrentPlayer) :-
    state_file(Path),
    setup_call_cleanup(
        open(Path, read, Stream),
        read_board_stream(Stream, Board, Size, CurrentPlayer),
        close(Stream)
    ).

/*
 * Helper for read_board_state/3 that consumes the stream in the order written
 * by the C++ code: first the board size, then each row, and finally the current
 * player identifier.
 */
read_board_stream(Stream, Board, Size, CurrentPlayer) :-
    read_line_to_codes(Stream, SizeCodes),
    number_codes(Size, SizeCodes),
    read_board_rows(Stream, Size, Board),
    read_line_to_codes(Stream, PlayerCodes),
    number_codes(CurrentPlayer, PlayerCodes).

/*
 * read_board_rows(+Stream, +N, -Rows)
 * Recursively read N lines and split each one into a list of integers.
 */
read_board_rows(_, 0, []) :- !.
read_board_rows(Stream, N, [Row|Rest]) :-
    read_line_to_codes(Stream, LineCodes),
    split_codes_to_numbers(LineCodes, Row),
    N1 is N - 1,
    read_board_rows(Stream, N1, Rest).

/*
 * split_codes_to_numbers(+Codes, -Numbers)
 * Convert a CSV line represented as character codes into a list of ints.
 */
split_codes_to_numbers(Codes, Numbers) :-
    string_codes(Str, Codes),
    split_string(Str, ",", " \t\r\n", Parts),
    maplist(number_string, Numbers, Parts).

/*
 * read_move_query(-Row, -Col, -Player)
 * Load the move requested by C++ so we can validate it.
 */
read_move_query(Row, Col, Player) :-
    query_file(Path),
    setup_call_cleanup(
        open(Path, read, Stream),
        (
            read_line_to_codes(Stream, LineCodes),
            string_codes(Line, LineCodes),
            split_string(Line, ",", " \t\r\n", [RowStr, ColStr, PlayerStr]),
            number_string(Row, RowStr),
            number_string(Col, ColStr),
            number_string(Player, PlayerStr)
        ),
        close(Stream)
    ).

/*
 * valid_position(+Index, +Size)
 * Ensure a row or column index falls within the board boundaries.
 */
valid_position(Index, Size) :-
    Index >= 0,
    Index < Size.

/*
 * is_empty(+Board, +Row, +Col)
 * Succeeds when the board cell holds zero.
 */
is_empty(Board, Row, Col) :-
    nth0(Row, Board, BoardRow),
    nth0(Col, BoardRow, 0).

/*
 * valid_player(+Player)
 * Both human and AI are represented as integer identifiers.
 */
valid_player(1).
valid_player(2).

/*
 * valid_move(+Board, +Row, +Col, +Player)
 * Combines all simple checks into one predicate. Anything more complicated can
 * be layered on top in future assignments.
 */
valid_move(Board, Row, Col, Player) :-
    length(Board, Size),
    valid_position(Row, Size),
    valid_position(Col, Size),
    valid_player(Player),
    is_empty(Board, Row, Col).

/*
 * all_same_nonzero(+List, -Value)
 * True when every element matches a non-zero value. Used to detect wins.
 */
all_same_nonzero([Value|Rest], Value) :-
    Value \= 0,
    maplist(=(Value), Rest).

/*
 * row_winner(+Board, -Winner)
 * Check each row for a winning line.
 */
row_winner(Board, Winner) :-
    member(Row, Board),
    all_same_nonzero(Row, Winner).

/*
 * column_values(+Board, +ColIndex, -Values)
 * Collect all values in a column so they can be tested by all_same_nonzero/2.
 */
column_values(Board, ColIndex, Values) :-
    findall(Value,
            ( nth0(_, Board, Row),
              nth0(ColIndex, Row, Value)
            ),
            Values).

/*
 * column_winner(+Board, -Winner)
 * Detect vertical wins by scanning every column index.
 */
column_winner(Board, Winner) :-
    length(Board, Size),
    between(0, Size - 1, ColIndex),
    column_values(Board, ColIndex, Values),
    all_same_nonzero(Values, Winner).

/*
 * diag_values(+Board, -Values)
 * Gather the main diagonal (top-left to bottom-right).
 */
diag_values(Board, Values) :-
    findall(Value,
            ( nth0(Index, Board, Row),
              nth0(Index, Row, Value)
            ),
            Values).

/*
 * anti_diag_values(+Board, -Values)
 * Gather the anti-diagonal (top-right to bottom-left).
 */
anti_diag_values(Board, Values) :-
    length(Board, Size),
    MaxIndex is Size - 1,
    findall(Value,
            ( nth0(Index, Board, Row),
              ColIndex is MaxIndex - Index,
              nth0(ColIndex, Row, Value)
            ),
            Values).

/*
 * check_winner(+Board, -Winner)
 * Try every win detection strategy until one succeeds.
 */
check_winner(Board, Winner) :-
    (   row_winner(Board, Winner)
    ;   column_winner(Board, Winner)
    ;   diag_values(Board, Diag),
        all_same_nonzero(Diag, Winner)
    ;   anti_diag_values(Board, AntiDiag),
        all_same_nonzero(AntiDiag, Winner)
    ).

/*
 * board_full(+Board)
 * True when there are no zeros left anywhere on the board.
 */
board_full(Board) :-
    \+ (member(Row, Board), member(0, Row)).

/*
 * game_over(+Board)
 * Useful helper predicate should we expand the Prolog feature set later.
 */
game_over(Board) :-
    check_winner(Board, _)
    ; board_full(Board).

/*
 * write_response(+Response)
 * Write the final verdict back to the file C++ reads after the swipl call.
 */
write_response(Response) :-
    response_file(Path),
    setup_call_cleanup(
        open(Path, write, Stream),
        ( write(Stream, Response), nl(Stream) ),
        close(Stream)
    ).

/*
 * validate_move/0 is the entry point invoked from the command line. Errors are
 * caught so the C++ caller always receives a response instead of hanging.
 */
validate_move :-
    catch(
        (
            read_board_state(Board, _Size, CurrentPlayer),
            read_move_query(Row, Col, Player),
            (   Player =:= CurrentPlayer,
                valid_move(Board, Row, Col, Player)
            ->  write_response(valid)
            ;   write_response(invalid)
            )
        ),
        Error,
        (
            print_message(error, Error),
            write_response(invalid)
        )
    ),
    !.

/*
 * Hook validate_move/0 up to the SWI-Prolog initialization mechanism so the
 * script behaves like a tiny command line tool.
 */
:- initialization(main).

main :-
    ( validate_move -> true ; true ),
    halt.

