;;; ai.scm
;;; Random-move Tic-Tac-Toe AI for the cross-language project
;;; This Scheme script acts like a tiny command line utility. C++ launches it,
;;; the script reads the board from disk, picks a move, writes the coordinates
;;; back, and exits. Keeping the logic simple makes the paradigm integration
;;; clear for newcomers.

;; Paths shared with the C++ orchestrator. Update here if the protocol changes.
(define state-file "data/current_state.txt")
(define response-file "data/move_response.txt")

;; split-csv takes a string like "0,1,0" and returns a list of element strings.
(define (split-csv line)
  (let loop ((chars (string->list line))
             (current '())
             (acc '()))
    (cond
      ((null? chars)
       (reverse (cons (list->string (reverse current)) acc)))
      ((char=? (car chars) #\,)
       (loop (cdr chars) '() (cons (list->string (reverse current)) acc)))
      ((char=? (car chars) #\,)
       (loop (cdr chars) '() (cons (list->string (reverse current)) acc)))
      (else
       (loop (cdr chars)
             (cons (car chars) current)
             acc)))))

;; parse-row converts a CSV line into actual numbers.
(define (parse-row line)
  (map string->number (split-csv line)))

;; read-board-state rebuilds the board structure produced by saveBoardState().
(define (read-board-state filename)
  (call-with-input-file filename
    (lambda (port)
      (let* ((size (string->number (read-line port)))
             (rows (let loop ((i 0) (acc '()))
                     (if (= i size)
                         (reverse acc)
                         (loop (+ i 1)
                               (cons (parse-row (read-line port)) acc)))))
             (current-player (string->number (read-line port))))
        (list size rows current-player)))))

;; board-ref is a tiny helper so lookups stay readable.
(define (board-ref board row col)
  (list-ref (list-ref board row) col))

;; get-valid-moves walks the board and collects every empty cell.
(define (get-valid-moves board)
  (let ((size (length board)))
    (let outer ((row 0) (acc '()))
      (if (= row size)
          (reverse acc)
          (let inner ((col 0) (inner-acc acc))
            (if (= col size)
                (outer (+ row 1) inner-acc)
                (if (= (board-ref board row col) 0)
                    (inner (+ col 1) (cons (list row col) inner-acc))
                    (inner (+ col 1) inner-acc))))))))

;; choose-random-move picks one of the available cells. Returning the empty list
;; signals that no moves are possible (board is full or game over).
(define (choose-random-move board)
  (let* ((moves (get-valid-moves board))
         (count (length moves)))
    (if (= count 0)
        '()
        (list-ref moves (random count)))))

;; write-move mirrors the format expected by the C++ code: "row,col".
(define (write-move move filename)
  (call-with-output-file filename
    (lambda (port)
      (if (or (null? move) (not (= (length move) 2)))
          (begin
            (display "-1,-1" port)
            (newline port))
          (begin
            (display (car move) port)
            (display "," port)
            (display (cadr move) port)
            (newline port))))))

;; main orchestrates the I/O steps and then terminates Scheme cleanly.
(define (main)
  (let* ((state (read-board-state state-file))
         (board (cadr state))
         (current-player (caddr state))
         (move (choose-random-move board)))
    ;; For now we ignore the current player value; we assume C++ only invokes us
    ;; when it is the AI's turn.
    (write-move move response-file)
    (exit)))

(main)

