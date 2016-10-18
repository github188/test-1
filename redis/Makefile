all:
	-@rm *.out
	@ctags -R *
	@gcc redis.c cJSON.c base64.c -lhiredis -lm -o redis.out
	@echo "-- OK! --"
