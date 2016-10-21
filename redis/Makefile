all:
	@echo "You should choose sgip_web or sgip_server, not all!"
sgip_web:
	@rm *.out
	@ctags -R *
	@gcc sgip_web.c cJSON.c base64.c -lhiredis -lm
	@echo "-- OK! --"
sgip_server:
	@rm *.out
	@ctags -R *
	@gcc sgip_server.c
	@echo "-- OK! --"
