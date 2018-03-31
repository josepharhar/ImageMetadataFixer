FLAGS = -std=gnu++11

fixer: fixer.cc
	g++ $(FLAGS) $< -o $@

.PHONY: clean
clean:
	-rm -rf fixer
