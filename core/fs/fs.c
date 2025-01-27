#include <stdio.h>

#include <vega/core/heap.h>

#include <vega/core/debug/tracy.h>

#include <vega/core/fs/fs.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

uint8_t fs_read_text(uint8_t** buffer, uint64_t* buffer_size, char const* file_path)
{
	TRACY_ZONE_BEGIN

	uint8_t read = 0;

	FILE* file = 0;

	fopen_s(&file, file_path, "r");

	if (file)
	{
		fseek(file, 0, SEEK_END);
		(*buffer_size) = (int unsigned)ftell(file);
		(*buffer) = (char*)heap_alloc(((*buffer_size) + 1));
		fseek(file, 0, SEEK_SET);

		fread((*buffer), sizeof(char), (*buffer_size), file);

		fclose(file);

		read = 1;
	}

	TRACY_ZONE_END

	return read;
}
uint8_t fs_read_binary(uint8_t** buffer, uint64_t* buffer_size, char const* file_path)
{
	TRACY_ZONE_BEGIN

	uint8_t read = 0;

	FILE* file = 0;

	fopen_s(&file, file_path, "rb");

	if (file)
	{
		fseek(file, 0, SEEK_END);
		(*buffer_size) = (int unsigned)ftell(file);
		(*buffer) = (char unsigned*)heap_alloc(*buffer_size);
		fseek(file, 0, SEEK_SET);

		fread((*buffer), sizeof(char unsigned), (*buffer_size), file);

		fclose(file);

		read = 1;
	}

	TRACY_ZONE_END

	return read;
}
uint8_t fs_write_text(uint8_t* buffer, uint64_t buffer_size, char const* file_path)
{
	TRACY_ZONE_BEGIN

	uint8_t written = 0;

	FILE* file = 0;

	fopen_s(&file, file_path, "w");

	if (file)
	{
		fseek(file, 0, SEEK_SET);

		fwrite(buffer, sizeof(char), buffer_size, file);

		fclose(file);

		written = 1;
	}

	TRACY_ZONE_END

	return written;
}
uint8_t fs_write_binary(uint8_t* buffer, uint64_t buffer_size, char const* file_path)
{
	TRACY_ZONE_BEGIN

	uint8_t written = 0;

	FILE* file = 0;

	fopen_s(&file, file_path, "wb");

	if (file)
	{
		fseek(file, 0, SEEK_SET);

		fwrite(buffer, sizeof(char unsigned), buffer_size, file);

		fclose(file);

		written = 1;
	}

	TRACY_ZONE_END

	return written;
}
string_t fs_resolve_file_stem_from_path(char const* file_path, uint64_t file_path_size)
{
	TRACY_ZONE_BEGIN

	uint64_t dot_marker = 0;

	uint64_t file_path_index = file_path_size;
	while (file_path_index >= 0)
	{
		if (file_path[file_path_index] == '.')
		{
			dot_marker = file_path_index;
		}

		if (file_path[file_path_index] == '\\' || file_path[file_path_index] == '/')
		{
			break;
		}

		file_path_index--;
	}

	string_t string = std_string_from(file_path + file_path_index + 1, dot_marker - file_path_index - 1);

	TRACY_ZONE_END

	return string;
}
string_t fs_resolve_file_ext_from_path(char const* file_path, uint64_t file_path_size)
{
	TRACY_ZONE_BEGIN

	uint64_t dot_marker = 0;

	uint64_t file_path_index = file_path_size;
	while (file_path_index >= 0)
	{
		if (file_path[file_path_index] == '.')
		{
			break;
		}

		file_path_index--;
	}

	string_t string = std_string_from(file_path + file_path_index + 1, file_path_size - file_path_index - 1);

	TRACY_ZONE_END

	return string;
}
string_t fs_resolve_file_root_from_path(char const* file_path, uint64_t file_path_size)
{
	TRACY_ZONE_BEGIN

	uint64_t file_path_index = file_path_size;
	while (file_path_index >= 0)
	{
		if (file_path[file_path_index] == '\\' || file_path[file_path_index] == '/')
		{
			break;
		}

		file_path_index--;
	}

	string_t string = std_string_from(file_path, file_path_index - 1);

	TRACY_ZONE_END

	return string;
}
