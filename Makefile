include VERSION.mk

cc=gcc
opts=-Wall -g -O3 -o
src_path=src
build_path=build
cflags=-I$(src_path)/include
libs=-L$(lib_dest_path)



lib_src_path=$(src_path)/lib
lib_dest_path=$(build_path)/lib
lib_srcs=$(wildcard $(lib_src_path)/*.c) $(wildcard $(lib_src_path)/*/*.c) $(wildcard $(lib_src_path)/*/*/*.c) $(wildcard $(lib_src_path)/*/*/*/*.c)
lib_objs=$(subst $(lib_src_path),$(lib_dest_path),$(lib_srcs:.c=.o))
lib_target=  $(lib_objs:.o=.a)
lib_libs=$(libs) $(addprefix -l,$(notdir $(subst .a,,$(lib_target))))
lib_cflags=$(cflags)


test_src_path=$(src_path)/test
test_dest_path=$(build_path)/test
test_srcs=$(wildcard $(test_src_path)/*.c) $(wildcard $(test_src_path)/*/*.c) $(wildcard $(test_src_path)/*/*/*.c) $(wildcard $(test_src_path)/*/*/*/*.c)
test_objs=$(subst $(test_src_path),$(test_dest_path),$(test_srcs:.c=.o))
test_target=$(subst .o,.exe,$(test_objs))
test_libs=$(lib_libs)
test_cflags=$(cflags)


app_src_path=$(src_path)/app
app_dest_path=$(build_path)/app
app_target=$(build_path)/kanban
app_srcs=$(wildcard $(app_src_path)/*.c) $(wildcard $(app_src_path)/*/*.c) $(wildcard $(app_src_path)/*/*/*.c) $(wildcard $(app_src_path)/*/*/*/*.c)
app_objs=$(subst $(app_src_path),$(app_dest_path),$(app_srcs:.c=.o))
app_libs=$(lib_libs)
app_cflags=$(cflags)

console_src_path=$(src_path)/console
console_dest_path=$(build_path)/console
console_target=$(build_path)/ckanban
console_srcs=$(wildcard $(console_src_path)/*.c) $(wildcard $(console_src_path)/*/*.c) $(wildcard $(console_src_path)/*/*/*.c) $(wildcard $(console_src_path)/*/*/*/*.c)
console_objs=$(subst $(console_src_path),$(console_dest_path),$(console_srcs:.c=.o))
console_libs=$(lib_libs)
console_cflags=$(cflags)



.PHONY: all build lib test console app

#### ALL rules ####
all: build
build: lib test console app

#### LIB rules ####
lib: $(lib_target)

$(lib_target):$(lib_objs)
	@echo "Build lib A: " $^ " => " $(dir $@)lib$(notdir $@)
	ar rcs -o $(dir $@)lib$(notdir $@) $^

$(lib_dest_path)/%.o:$(lib_src_path)/%.c
	@echo "Create lib O: " $^ " => " $@
	mkdir -p $(dir $@)
	$(cc) $(opts) $@ -c $^ $(lib_cflags) $(lib_libs)



#### TEST rules ####
test: lib $(test_target)

$(test_dest_path)/%.exe:$(test_dest_path)/%.o
	@echo "Build test EXE: " $^ " => " $@
	$(cc) $(opts) $@ $^ $(test_cflags) $(test_libs)


$(test_dest_path)/%.o:$(test_src_path)/%.c
	@echo "Create test O: " $^ " => " $@
	mkdir -p $(dir $@)
	$(cc) $(opts) $@ -c $^ $(test_cflags) $(test_libs)



#### CONSOLE rules ####
console: lib $(console_target)

$(console_target):$(console_objs)
	@echo "Build console EXE: " $^ " => " $@
	$(cc) $(opts) $@ $^ $(test_cflags) $(test_libs)


$(console_dest_path)/%.o:$(console_src_path)/%.c
	mkdir -p $(dir $@)
	@echo "Create console O: " $^ " => " $@
	$(cc) $(opts) $@ -c $^ $(console_cflags) $(console_libs)




#### APP rules ####
app: lib $(app_target)

$(app_target):$(app_objs)
	@echo "Build app EXE: " $^ " => " $@

$(app_dest_path)/%.o:$(app_src_path)/%.c
	@echo mkdir -p $(dir $@)
	@echo "Create app O: " $^ " => " $@



#### SYSTEM rules ####
clean:
	rm -rf $(build_path)

display:
	@echo "=== GLOBAL ==="
	@echo "cc" $(cc)
	@echo "opts" $(opts)
	@echo "cflags" $(cflags)
	@echo "libs" $(libs)
	@echo
	@echo "=== APP ==="
	@echo "app_src_path" $(app_src_path)
	@echo "app_dest_path" $(app_dest_path)
	@echo "app_target" $(app_target)
	@echo "app_srcs" $(app_srcs)
	@echo "app_objs" $(app_objs)
	@echo "app_libs" $(app_libs)
	@echo "app_cflags" $(app_cflags)
	@echo
	@echo "=== CONSOLE ==="
	@echo "console_src_path" $(console_src_path)
	@echo "console_dest_path" $(console_dest_path)
	@echo "console_target" $(console_target)
	@echo "console_srcs" $(console_srcs)
	@echo "console_objs" $(console_objs)
	@echo "console_libs" $(console_libs)
	@echo "console_cflags" $(console_cflags)
	@echo
	@echo "=== LIB ==="
	@echo "lib_src_path" $(lib_src_path)
	@echo "lib_dest_path" $(lib_dest_path)
	@echo "lib_srcs" $(lib_srcs)
	@echo "lib_objs" $(lib_objs)
	@echo "lib_target" $(lib_target)
	@echo "lib_libs" $(lib_libs)
	@echo "lib_cflags" $(lib_cflags)
	@echo
	@echo "=== TEST ==="
	@echo "test_src_path" $(test_src_path)
	@echo "test_dest_path" $(test_dest_path)
	@echo "test_srcs" $(test_srcs)
	@echo "test_objs" $(test_objs)
	@echo "test_target" $(test_target)
	@echo "test_libs" $(test_libs)
	@echo "test_cflags" $(test_cflags)

version:
	@echo "lib_kanban_version" $(LIB_KANBAN_VERSION)
	@echo "console_version" $(CONSOLE_VERSION)
	@echo "app_version" $(APP_VERSION)
	@echo "build_version" $(BUILD_VERSION)
	@echo "SPDX-License-Identifier" SPDX_LICENSE