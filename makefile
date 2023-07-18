VMTranslator: *.cpp
	g++ -Wall *.cpp -o VMTranslator -std=c++17 -lstdc++fs
	chmod +x VMTranslator