include compiler.config
old_path:=${PATH}

usage:
	@echo -e "Usage: make <program>\n\
		where program is one of the following:\n\
			fb2toepub\n\
			unrar\n\
			7za\n\
			dictionary"
			
clean:
	rm -fR dist

init:
	@mkdir -p dist
	
all: fb2toepub rar 7za dictionary
	
fb2toepub unrar 7za dictionary : init
	rm -f dist/$@*
	@echo "=== Building $@ for 300 & 505 ==="
	@cd src/$@; make clean # need to clean 600 objs
	@cd src/$@; make PATH=$(path_300):$(old_path) ${vars_300}
	@mv src/$@/$@ dist/$@.300
	@echo "=== Done building $@ for 300 & 505 ==="
	@echo "=== Building dictionary for 600 & x50... ==="
	@cd src/$@; make clean # need to clean 300 objs
	@cd src/$@; make PATH=$(path_600):$(old_path) ${vars_600}
	@echo "=== Done building dictionary for 600 & x50 ==="
	@mv src/$@/$@ dist/$@.600
	@cp dist/$@.300 dist/$@.505
	@cp dist/$@.600 dist/$@.350
	@cp dist/$@.600 dist/$@.650
	@cp dist/$@.600 dist/$@.950

