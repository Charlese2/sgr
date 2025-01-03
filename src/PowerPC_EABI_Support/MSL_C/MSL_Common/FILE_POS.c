#include "FILE_POS.h"
#include "buffer_io.h"
#include "critical_regions.h"
#include "errno.h"

long ftell(FILE* stream) {
    int retval;
    retval = (long)_ftell(stream);
    return retval;
}

inline int _ftell(FILE* file) {
    int charsInUndoBuffer = 0;
    int position;

    unsigned char tmp_kind = file->file_mode.file_kind;
    if (!(tmp_kind == __disk_file || tmp_kind == __console_file) || file->file_state.error) {
        errno = 0x28;
        return -1;
    }

    if (file->file_state.io_state == __neutral)
        return (file->position);

    position = file->buffer_position + (file->buffer_ptr - file->buffer);

    if (file->file_state.io_state >= __rereading) {
        charsInUndoBuffer = file->file_state.io_state - __rereading + 1;
        position -= charsInUndoBuffer;
    }

    return (position);
}

int _fseek(FILE* file, unsigned long offset, int whence) {
    int bufferCode;
    int pos;
    int adjust;
    unsigned long state;
    int buffLen;

    unsigned char* ptr;

    if (file->file_mode.file_kind != 1 || file->file_state.error != 0) {
        errno = 0x28;
        return -1;
    }

    if (file->file_state.io_state == 1) {
        if (__flush_buffer(file, NULL) != 0) {
            file->file_state.error = 1;
            file->buffer_length = 0;
            errno = 0x28;
            return -1;
        }
    }

    if (whence == SEEK_CUR) {
        whence = SEEK_SET;
        offset += _ftell(file);
    }

    if ((whence != SEEK_END) && (file->file_mode.io_mode != 3) &&
        (file->file_state.io_state == 2 || file->file_state.io_state == 3))
    {
        if ((offset >= file->position) || !(offset >= file->buffer_position)) {
            file->file_state.io_state = 0;
        } else {
            file->buffer_ptr = file->buffer + (offset - file->buffer_position);
            file->buffer_length = file->position - offset;
            file->file_state.io_state = 2;
        }
    } else {
        file->file_state.io_state = 0;
    }

    if (file->file_state.io_state == 0) {
        if (file->position_fn != NULL &&
            (int)file->position_fn(file->handle, &offset, whence, file->idle_fn))
        {
            file->file_state.error = 1;
            file->buffer_length = 0;
            errno = 0x28;
            return -1;
        } else {
            file->file_state.eof = 0;
            file->position = offset;
            file->buffer_length = 0;
        }
    }

    return 0;
}

int fseek(FILE* stream, unsigned long offset, int whence) {
    int code;
    code = _fseek(stream, offset, whence);
    return code;
}
