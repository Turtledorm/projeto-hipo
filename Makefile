.PHONY: push pull amend

all:
	gcc -Wall -O3 -ansi -pedantic -Wno-unused-result Montador.c -o Montador

push:
	git add . && git commit -m "$(M)" && git push

pull:
	git pull origin master

amend:
	git add . && git commit --amend && git push -f
