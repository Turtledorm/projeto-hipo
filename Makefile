.PHONY: push pull amend

push:
	git add . && git commit -m "$(M)" && git push

pull:
	git pull origin master

amend:
	git add . && git commit --amend && git push -f
