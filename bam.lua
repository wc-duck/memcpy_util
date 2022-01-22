--[[
 Header containing utility functions for copying memory in different ways.

 Most functions exist in a memcpy_- and a memmove_-version where the move
 versions trade performance for being able to work within overlapping
 memory regions.

 version 1.0, January, 2021

 Copyright (C) 2021- Fredrik Kihlander

 This software is provided 'as-is', without any express or implied
 warranty.  In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.

 Fredrik Kihlander
--]]

BUILD_PATH = "local"

config   = ScriptArgs["config"]
compiler = "clang"
compiler = ScriptArgs["compiler"]

local settings = NewSettings() -- {}

if compiler == "clang" then
        SetDriversClang(settings)
elseif compiler == "gcc" then
        SetDriversGCC(settings)
end

settings.cc.includes:Add("include")
if family == 'windows' then
	platform = "winx64"
else
	platform = "linux_x86_64"
	settings.cc.flags:Add( "-Wconversion", "-Wextra", "-Wall", "-Werror", "-Wstrict-aliasing=2", "-" .. config )
end

local output_path = PathJoin( BUILD_PATH, PathJoin( platform, PathJoin( compiler, config ) ) )
local output_func = function(settings, path) return PathJoin(output_path, PathFilename(PathBase(path)) .. settings.config_ext) end
settings.cc.Output = output_func
settings.lib.Output = output_func
settings.link.Output = output_func

local tests  = Link( settings, 'memcpy_util_test',  Compile( settings, 'test/memcpy_util_test.cpp' ) )
local bench  = Link( settings, 'memcpy_util_bench', Compile( settings, 'bench/memcpy_util_bench.cpp' ) )

test_args = " -v"
if ScriptArgs["test"]  then test_args = test_args .. " -t " .. ScriptArgs["test"] end
if ScriptArgs["suite"] then test_args = test_args .. " -s " .. ScriptArgs["suite"] end
if ScriptArgs["bench"] then test_args = test_args .. " -t " .. ScriptArgs["bench"] end

if family == "windows" then
        AddJob( "test",  "unittest",  string.gsub( tests, "/", "\\" ) .. test_args, tests, tests )
        AddJob( "bench", "bench",     string.gsub( bench, "/", "\\" ) .. test_args, bench, bench )
else
        AddJob( "test",     "unittest",  tests .. test_args, tests, tests )
        AddJob( "valgrind", "valgrind",  "valgrind -v --leak-check=full --track-origins=yes " .. tests .. test_args, tests, tests )
        AddJob( "bench",    "bench",  bench .. test_args, bench, bench )
end

PseudoTarget( "all", tests, bench )
DefaultTarget( "all" )
