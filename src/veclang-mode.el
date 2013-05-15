;; Copyright (C) 2013 BAH Salmane, CHALAUX Benjamin , SOLLAUD Timothée

;; This file is part of Veclang: a toy compiler for vectorial drawing using the cairo library
;;    Veclang is free software: you can redistribute it and/or modify
;;    it under the terms of the GNU General Public License as published by
;;    the Free Software Foundation, either version 3 of the License, or
;;    (at your option) any later version.

;;    Veclang is distributed in the hope that it will be useful,
;;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;    GNU General Public License for more details.

;;    You should have received a copy of the GNU General Public License
;;    along with Veclang.  If not, see <http://www.gnu.org/licenses/>


;; regexp string for veclang symbol class
(setq veclang-keywords-regexp (regexp-opt '("if" "else" "image" "execute") 'words))
(setq veclang-types-regexp (regexp-opt '("scal" "path" "pict") 'words))
(setq veclang-functions-regexp (regexp-opt '("rotate" "translate" "draw" "fill") 'words))

;; list for font-lock

(setq veclang-font-lock-keywords
      `((,veclang-types-regexp . font-lock-type-face)
	(,veclang-functions-regexp . font-lock-function-name-face)
	(,veclang-keywords-regexp . font-lock-keyword-face)))



;; handling completion

(setq veclang-keyword-list '(
			     "if"
			     "else"
			     "image"
			     "execute"
			     "scal"
			     "path"
			     "pict"
			     "rotate"
			     "translate"
			     "draw"
			     "fill"))

(defun veclang-complete-symbol ()
  "keyword completion on word before cursor"
  (interactive)
  (let ((pos-end (point))
	(word (thing-at-point 'symbol))
	max-match-result)
    (when (not word)
      (setq word ""))
    (setq max-match-result (try-completion word veclang-keyword-list))
    
    (cond ((eq max-match-result t))
	  ((null max-match-result)
	  (message "Can't find completion for %s" word)
	  (ding))
	  ((not (string= word max-match-result))
	   (delete-region (- pos-end (length word)) pos-end)
	   (insert max-match-result))
	  (t (message "Making completion list...")
	     (with-output-to-temp-buffer "*Completions*"
	       (display-completion-list (all-completions word veclang-keyword-list)
					word))
	     (message "Making completion list...%s" "done")))))

(define-derived-mode veclang-mode c-mode
  "major mode for editing veclang"
  (setq font-lock-defaults '((veclang-font-lock-keywords))))

(provide 'veclang-mode)
