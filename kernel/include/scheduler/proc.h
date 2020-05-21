#pragma once

typedef void (*job_t)();

void
start_kernel_process(job_t job);
