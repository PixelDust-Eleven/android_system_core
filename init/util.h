/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _INIT_UTIL_H_
#define _INIT_UTIL_H_

#include <sys/stat.h>
#include <sys/types.h>

#include <chrono>
#include <string>
#include <functional>

#define COLDBOOT_DONE "/dev/.coldboot_done"

using namespace std::chrono_literals;

int create_socket(const char *name, int type, mode_t perm,
                  uid_t uid, gid_t gid, const char *socketcon);
int create_file(const char *path, int mode, mode_t perm,
                uid_t uid, gid_t gid, const char *filecon);

bool read_file(const char* path, std::string* content);
int write_file(const char* path, const char* content);

// A std::chrono clock based on CLOCK_BOOTTIME.
class boot_clock {
 public:
  typedef std::chrono::nanoseconds duration;
  typedef std::chrono::time_point<boot_clock, duration> time_point;
  static constexpr bool is_steady = true;

  static time_point now();
};

class Timer {
 public:
  Timer() : start_(boot_clock::now()) {
  }

  double duration() {
    typedef std::chrono::duration<double> double_duration;
    return std::chrono::duration_cast<double_duration>(boot_clock::now() - start_).count();
  }

 private:
  boot_clock::time_point start_;
};

unsigned int decode_uid(const char *s);

int mkdir_recursive(const char *pathname, mode_t mode);
void sanitize(char *p);
int wait_for_file(const char *filename, std::chrono::nanoseconds timeout);
void import_kernel_cmdline(bool in_qemu,
                           const std::function<void(const std::string&, const std::string&, bool)>&);
int make_dir(const char *path, mode_t mode);
int restorecon(const char *pathname);
int restorecon_recursive(const char *pathname);
std::string bytes_to_hex(const uint8_t *bytes, size_t bytes_len);
bool is_dir(const char* pathname);
bool expand_props(const std::string& src, std::string* dst);
#endif
