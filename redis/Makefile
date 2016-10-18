all:
	-@rm *.out
	@ctags -R *
	@gcc redis.c cJSON.c -lhiredis -lm -o redis.out
	@echo "-- OK! --"
