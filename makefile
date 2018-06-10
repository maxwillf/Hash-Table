All:
	@echo "Compiling Hash Program..."
	@g++ -w -std=c++11 src/* -I include -o bin/hash
	@echo "For running the Program"
	@echo "Use: ./bin/hash"
clean:
	@echo "Cleaning..."
	@rm bin/*