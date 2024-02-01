/*
 *  emscripten_err_desc.c
 *  artoolkitX
 *
 *  This file is part of artoolkitX.
 *
 *  artoolkitX is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  artoolkitX is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with artoolkitX.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  As a special exception, the copyright holders of this library give you
 *  permission to link this library with independent modules to produce an
 *  executable, regardless of the license terms of these independent modules, and to
 *  copy and distribute the resulting executable under terms of your choice,
 *  provided that you also meet, for each linked independent module, the terms and
 *  conditions of the license of that module. An independent module is a module
 *  which is neither derived from nor based on this library. If you modify this
 *  library, you may extend this exception to your version of the library, but you
 *  are not obligated to do so. If you do not wish to do so, delete this exception
 *  statement from your version.
 *
 *  Copyright 2023-2024 Philip Lamb
 *
 *  Author(s): Philip Lamb
 */

#include "emscripten_err_desc.h"
#include <stdio.h>

struct _emscripten_err_desc {
	EMSCRIPTEN_RESULT err;
	const char *desc; 
};

const static struct _emscripten_err_desc emscripten_err_descs[] = {
	{EMSCRIPTEN_RESULT_SUCCESS, "The operation succeeded."},
	{EMSCRIPTEN_RESULT_DEFERRED, "The requested operation cannot be completed now for web security reasons, and has been deferred for completion in the next event handler."},
	{EMSCRIPTEN_RESULT_NOT_SUPPORTED, "The given operation is not supported by this browser or the target element. This value will be returned at the time the callback is registered if the operation is not supported."},
	{EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED, "The requested operation could not be completed now for web security reasons. It failed because the user requested the operation not be deferred."},
	{EMSCRIPTEN_RESULT_INVALID_TARGET, "The operation failed because the specified target element is invalid."},
	{EMSCRIPTEN_RESULT_UNKNOWN_TARGET, "The operation failed because the specified target element was not found."},
	{EMSCRIPTEN_RESULT_INVALID_PARAM, "The operation failed because an invalid parameter was passed to the function."},
	{EMSCRIPTEN_RESULT_FAILED, "Generic failure result message, returned if no specific result is available."},
	{EMSCRIPTEN_RESULT_NO_DATA, "The operation failed because no data is currently available."},
};
#define EMSCRIPTEN_ERR_DESC_COUNT sizeof(emscripten_err_descs)/sizeof(struct _emscripten_err_desc)

const char *emscripten_err_desc(const EMSCRIPTEN_RESULT err)
{
	for (int i = 0; i < EMSCRIPTEN_ERR_DESC_COUNT; i++) {
		if (emscripten_err_descs[i].err == err) return emscripten_err_descs[i].desc;
	}
	return NULL;
}