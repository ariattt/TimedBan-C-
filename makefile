args = `arg="$(filter-out $@,$(MAKECMDGOALS))" && echo $${arg:-${1}}`

all:
	g++ pro.cc -o ha
	./ha

ban:
	g++ pro.cc -o ha
	./ha $(call args)

clear:
	rm ha
