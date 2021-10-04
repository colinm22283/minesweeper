all:
	g++ -I ./header/ ./source/*.cpp -o ./minesweeper
	./minesweeper
compile:
	g++ -I ./header/ ./source/*.cpp -o ./minesweeper
run:
	./minesweeper
push:
	git add ./header/*.h ./source/*.cpp ./Makefile ./README.md
	git commit -m "Added a working pause menu"
	git push -u origin main