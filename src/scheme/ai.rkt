#lang racket

;; import list utilities and string utilities
(require racket/list
         racket/string)

;; store the file path where the board state is written
(define state-file "data/current_state.txt")

;; store the file path where the AI writes its move
(define response-file "data/move_response.txt")

;; constant representing an empty board space
(define EMPTY 0)

;; constant representing the human player (player 1)
(define HUMAN 1)

;; constant representing the AI player (player 2)
(define AI 2)

;; splits a comma separated string into a list of substrings
(define (split-csv line)
  ;; call string-split with comma delimiter
  (string-split line ","))

;; converts a CSV string row into a list of numbers
(define (parse-row line)
  ;; map string->number across the split fields
  (map string->number (split-csv line)))

;; reads the board state from file and returns (list size rows current-player)
(define (read-board-state filename)
  ;; open filename for input
  (call-with-input-file filename
    (lambda (in)
      ;; read the first line as board size
      (define size (string->number (read-line in)))
      ;; read size number of lines into row lists
      (define rows
        (for/list ([i (in-range size)])
          (parse-row (read-line in))))
      ;; read last line as current player
      (define current-player (string->number (read-line in)))
      ;; return structure as a list
      (list size rows current-player))))

;; retrieves the value at a given row and column in the board
(define (board-ref board row col)
  ;; get row first then fetch the column value
  (list-ref (list-ref board row) col))

;; gathers all empty cells on the board as (row col) pairs
(define (get-valid-moves board)
  ;; compute size of board
  (define size (length board))
  ;; build list of valid coordinates
  (for*/list ([r (in-range size)]
              [c (in-range size)]
              #:when (= (board-ref board r c) EMPTY))
    (list r c)))

;; replaces one cell in the board and returns a new board
(define (set-cell board row col value)
  ;; map across rows while injecting updated row at proper index
  (for/list ([r-idx (in-naturals)]
             [row-list board])
    ;; check if this is the row we want to modify
    (if (= r-idx row)
        ;; modify the target row by replacing one element
        (for/list ([c-idx (in-naturals)]
                   [cell row-list])
          (if (= c-idx col) value cell))
        ;; otherwise return the row unchanged
        row-list)))

;; applies a move for a player and returns the resulting board
(define (apply-move board move player)
  ;; extract row coordinate
  (define row (first move))
  ;; extract column coordinate
  (define col (second move))
  ;; set the given cell to player's value
  (set-cell board row col player))

;; checks a list to see if all values match a non-zero player
(define (line-winner line)
  ;; ensure list is not empty
  (and (not (null? line))
       ;; store first element as candidate
       (let ([v (first line)])
         ;; require non-empty value
         (and (not (= v EMPTY))
              ;; ensure entire line matches candidate
              (andmap (λ (x) (= x v)) line)
              ;; return candidate as winner
              v))))

;; checks if any row, column, or diagonal yields a winner
(define (winner board)
  ;; determine size of board
  (define size (length board))
  ;; store rows directly
  (define rows board)
  ;; build list of columns
  (define cols
    (for/list ([c (in-range size)])
      (for/list ([r (in-range size)])
        (board-ref board r c))))
  ;; build main diagonal list
  (define main-diag
    (for/list ([i (in-range size)])
      (board-ref board i i)))
  ;; build anti-diagonal list
  (define anti-diag
    (for/list ([i (in-range size)])
      (board-ref board i (- size 1 i))))
  ;; test each row, column, and diagonal for a winner
  (or (for/or ([row rows]) (line-winner row))
      (for/or ([col cols]) (line-winner col))
      (line-winner main-diag)
      (line-winner anti-diag)))

;; checks whether the board has any empty spaces left
(define (board-full? board)
  ;; ensure every row has no zero values
  (for/and ([row board])
    (for/and ([cell row])
      (not (= cell EMPTY)))))

;; checks whether this board is terminal (win or full)
(define (terminal? board)
  ;; terminal if there is a winner or all spaces are full
  (or (winner board)
      (board-full? board)))

;; assigns a numeric score to a terminal board position
(define (score-position board)
  ;; get winner of board
  (define w (winner board))
  ;; return AI win as +1
  ;; return HUMAN win as -1
  ;; return 0 for draw or non-final call
  (cond
    [(not w) 0]
    [(= w AI) 1]
    [(= w HUMAN) -1]
    [else 0]))

;; performs minimax search for best possible score for the given player
(define (minimax board player)
  ;; handle case when someone has won
  (cond
    [(winner board) (score-position board)]
    ;; handle draw state
    [(board-full? board) 0]
    [else
     ;; compute list of possible moves
     (define moves (get-valid-moves board))
     ;; compute score of each possible move
     (define scores
       (for/list ([mv moves])
         ;; apply move to create new board
         (define new-board (apply-move board mv player))
         ;; recursively run minimax for opposite player
         (minimax new-board (if (= player AI) HUMAN AI))))
     ;; if AI turn choose maximum score
     (if (= player AI)
         (apply max scores)
         ;; if HUMAN turn choose minimum score
         (apply min scores))]))

;; determines the best move for the AI based on minimax scoring
(define (best-move board)
  ;; get list of all possible moves
  (define moves (get-valid-moves board))
  ;; if no moves possible return empty
  (if (null? moves)
      '()
      ;; score each possible move
      (let* ([scored
              ;; build list of (move . score)
              (for/list ([mv moves])
                ;; apply move for AI
                (define new-board (apply-move board mv AI))
                ;; compute score for resulting board
                (define s (minimax new-board HUMAN))
                ;; return cons pair
                (cons mv s))]
             ;; choose highest scoring move
             [best
              (foldl (λ (a b) (if (> (cdr a) (cdr b)) a b))
                     (first scored)
                     (rest scored))])
        ;; return move with highest score
        (car best))))

;; writes the chosen move to a file in "row,col" format
(define (write-move move filename)
  ;; write output using call-with-output-file
  (call-with-output-file filename
    (lambda (out)
      ;; write invalid move if empty structure returned
      (if (or (null? move) (not (= (length move) 2)))
          (fprintf out "-1,-1\n")
          ;; otherwise print "row,col"
          (fprintf out "~a,~a\n" (first move) (second move))))
    ;; replace existing file
    #:exists 'replace))

;; main function that loads state, computes move, and writes output
(define (main)
  ;; read current state from file
  (define state (read-board-state state-file))
  ;; extract board rows from state
  (define board (second state))
  ;; compute best move using minimax
  (define move (best-move board))
  ;; write the selected move to file
  (write-move move response-file)
  ;; exit the program
  (exit 0))

;; invoke main when file is executed
(main)