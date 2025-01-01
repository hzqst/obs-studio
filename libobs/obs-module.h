/******************************************************************************
    Copyright (C) 2023 by Lain Bailey <lain@obsproject.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#pragma once

#include "obs.h"

#ifdef __cplusplus
#define MODULE_EXPORT extern "C" EXPORT
#define MODULE_EXTERN extern "C"
#else
#define MODULE_EXPORT EXPORT
#define MODULE_EXTERN extern
#endif

/**
 * @file
 * @brief This file is used by modules for module declaration and module
 *        exports.
 *
 * @page modules_page Modules
 * @brief Modules or plugins are libraries that can be loaded by libobs and
 *        subsequently interact with it.
 *
 * @section modules_overview_sec Overview
 *
 * Modules can provide a wide range of functionality to libobs, they for example
 * can feed captured audio or video to libobs, or interface with an encoder to
 * provide some codec to libobs.
 *
 * @section modules_basic_sec Creating a basic module
 *
 * In order to create a module for libobs you will need to build a shared
 * library that implements a basic interface for libobs to interact with.
 * The following code would create a simple source plugin without localization:
 *
@code
#include <obs-module.h>

OBS_DECLARE_MODULE()

extern struct obs_source_info my_source;

bool obs_module_load(void)
{
	obs_register_source(&my_source);
	return true;
}
@endcode
 *
 * If you want to enable localization, you will need to also use the
 * @ref OBS_MODULE_USE_DEFAULT_LOCALE() macro.
 *
 * Other module types:
 * - @ref obs_register_encoder()
 * - @ref obs_register_service()
 * - @ref obs_register_output()
 *
 */

/**
 * Required: Called when the module is loaded.  Use this function to load all
 * the sources/encoders/outputs/services for your module, or anything else that
 * may need loading.
 *
 * @return           Return true to continue loading the module, otherwise
 *                   false to indicate failure and unload the module
 */
static bool obs_module_load(void);

/** Optional: Called when the module is unloaded.  */
static void obs_module_unload(void);

/** Optional: Called when all modules have finished loading */
static void obs_module_post_load(void);

/** Called to set the current locale data for the module.  */
static void obs_module_set_locale(const char *locale);

/** Called to free the current locale data for the module.  */
static void obs_module_free_locale(void);

static const char *obs_module_text_impl(const char *val);

static bool obs_module_get_string_impl(const char *val, const char **out);

/** Required: Declares a libobs module. */
#define OBS_DECLARE_MODULE(module_name) obs_module_t *obs_current_module_##module_name(void);

#define OBS_DEFINE_MODULE(module_name)                                             \
	static const char* obs_module_name(void);\
	static const char *obs_module_description(void);\
	static const char *obs_module_author(void);\
	static uint32_t obs_module_ver(void);				\
	static void obs_module_set_pointer(obs_module_t *mod); \
	static obs_module_t obs_module_info = {\
		.mod_name = #module_name,                             \
						      .file = NULL,            \
						      .bin_path = NULL,              \
						      .load = obs_module_load,                   \
						      .unload = obs_module_unload,               \
						      .post_load = obs_module_post_load,         \
						      .set_locale = obs_module_set_locale,       \
							.text = obs_module_text_impl,               \
						      .get_string = obs_module_get_string_impl,               \
						      .free_locale = obs_module_free_locale,       \
						      .ver = obs_module_ver, \
						      .set_pointer = obs_module_set_pointer,           \
						      .name = obs_module_name,  \
						      .description = obs_module_description,  \
						      .author = obs_module_author,  \
						      .next = NULL};         \
	static obs_module_t *obs_module_pointer = &obs_module_info;                         \
	static void obs_module_set_pointer(obs_module_t *mod)                \
	{                                                                \
		obs_module_pointer = mod;                             \
	}                                                                \
	static uint32_t obs_module_ver(void)                                          \
	{                                                                             \
		return LIBOBS_API_VER;                                                \
	}\
	obs_module_t *obs_current_module_##module_name(void)                    \
	{                                                                \
		return obs_module_pointer;                        \
	}                                   

/** Optional: Use this macro in a module to use default locale handling. */
#define OBS_MODULE_USE_DEFAULT_LOCALE(module_name, default_locale)                                        \
	static lookup_t *obs_module_lookup = NULL;                                                               \
	static const char *obs_module_text_impl(const char *val)                                                      \
	{                                                                                                 \
		const char *out = val;                                                                    \
		text_lookup_getstr(obs_module_lookup, val, &out);                                         \
		return out;                                                                               \
	}                                                                                                 \
	static bool obs_module_get_string_impl(const char *val, const char **out)                                     \
	{                                                                                                 \
		return text_lookup_getstr(obs_module_lookup, val, out);                                   \
	}                                                                                                 \
	static void obs_module_set_locale(const char *locale)                                                    \
	{                                                                                                 \
		if (obs_module_lookup)                                                                    \
			text_lookup_destroy(obs_module_lookup);                                           \
		obs_module_lookup =                                                                         \
			obs_module_load_locale(obs_current_module_##module_name(), default_locale, locale); \
	}                                                                                                 \
	static void obs_module_free_locale(void)                                                                 \
	{                                                                                                 \
		text_lookup_destroy(obs_module_lookup);                                                   \
		obs_module_lookup = NULL;                                                                 \
	}

/** Helper function for looking up locale if default locale handler was used */
#define obs_module_text(module_name, lookup_string) obs_current_module_##module_name()->text(lookup_string)

/** Helper function for looking up locale if default locale handler was used,
 * returns true if text found, otherwise false */
#define obs_module_get_string(module_name, lookup_string, translated_string) obs_current_module_##module_name()->get_string(lookup_string, translated_string)

/** Helper function that returns the current module */
//obs_module_t *obs_current_module_##(void);

/**
 * Returns the location to a module data file associated with the current
 * module.  Free with bfree when complete.  Equivalent to:
 *    obs_find_module_file(obs_current_module(), file);
 */
#define obs_module_file(module_name, file) obs_find_module_file(obs_current_module_##module_name(), file)

/**
 * Returns the location to a module config file associated with the current
 * module.  Free with bfree when complete.  Will return NULL if configuration
 * directory is not set.  Equivalent to:
 *    obs_module_get_config_path(obs_current_module(), file);
 */
#define obs_module_config_path(module_name, file) obs_module_get_config_path(obs_current_module_##module_name(), file)
