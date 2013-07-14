package = 'oocairo'
version = '1-2'

source = {
	url = 'git+file://git@github.com:akfidjeland/torch-oocairo.git',
	branch = 'torch'
}

description = {
    summary = 'Lua bindings for Cairo, with extensions for Torch',
	homepage = 'git+file://git@github.com:akfidjeland/torch-oocairo.git'
}

dependencies = {
    'torch >= 7.0'
}

build = {
	type = "command",
	build_command = [[
cmake -E make_directory build;
cd build;
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$(LUA_BINDIR)/.." -DCMAKE_INSTALL_PREFIX="$(PREFIX)"; 
$(MAKE)
   ]],
   install_command = "cd build && $(MAKE) install"
}
