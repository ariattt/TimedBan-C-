args = `arg="$(filter-out $@,$(MAKECMDGOALS))" && echo $${arg:-${1}}`

all:
	g++ -std=c++14 pro.cc -o ha
	./ha

ban:
	g++ -std=c++14 pro.cc -o ha
	./ha $(call args)

clear:
	rm ha
