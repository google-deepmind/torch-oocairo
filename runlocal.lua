-- This will load the new copy of the library on Unix systems where it's built
-- with libtool.
package.cpath = ".libs/liblua-?.so;" .. package.cpath

local _tmp_filenames = {}
function tmpname ()
    local filename = os.tmpname()
    _tmp_filenames[#_tmp_filenames + 1] = filename
    return filename
end

function clean_up_temp_files ()
    for _, filename in ipairs(_tmp_filenames) do
        assert(os.remove(filename))
    end
    _tmp_filenames = {}
end

-- vi:ts=4 sw=4 expandtab
