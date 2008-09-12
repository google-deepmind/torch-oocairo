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

-- Arbitrary drawing just to make sure there's something in the output file.
function draw_arbitrary_stuff (Cairo, surface)
    local cr = Cairo.context_create(surface)
    cr:move_to(50, 50)
    cr:line_to(250, 150)
    cr:set_line_width(5)
    cr:set_source_rgb(1, 0, 0.5)
    cr:stroke()
end

-- vi:ts=4 sw=4 expandtab
