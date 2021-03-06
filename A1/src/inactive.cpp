/*
 * CSC469 - Performance Evaluation
 *
 * Daniel Bloemendal <d.bloemendal@gmail.com>
 */

#include "perftest.hpp"

// System includes
#include <time.h>

/*
 * inactive_periods_ex
 *
 * Collect the lengths of inactive periods
 */
u_int64_t inactive_periods(int num, u_int64_t threshold, std::vector<period_t>& samples) {
    // Reserve capacity for the samples
    samples.reserve(samples.size() + num);

    // Cycle counter instance
    TSC counter;
    counter.start();

    // Get starting cycle count
    TSC::cycles start = counter.count();

    // The inactive period sampler loop
    for(int i = 0; i < num; i++) {
        // Active and inactive periods
        period_t active, inactive;

        // Set initial start and end
        active.start = (i != 0) ? counter.count() : start;
        active.end   = active.start;

        // Loop until inactive period detected
        while(true) {
            // Current cycle count
            TSC::cycles current = counter.count();

            // See if number of cycles elapsed passes threshold
            if(current - active.end >= threshold) {
                // Record the inactive period
                inactive.start = active.end;
                inactive.end   = current;
                samples.push_back(inactive);

                // We can break out of the detection loop now
                break;
            }

            // Set new ending cycle count
            active.end = current;
        }
    }

    // Return the start time
    return start;
}

/*
 * inactive_periods
 *
 * Collect the lengths of inactive periods
 */
u_int64_t inactive_periods(int num, u_int64_t threshold, u_int64_t* samples) {
    std::vector<period_t> results;
    u_int64_t start = inactive_periods(num, threshold, results);
    for(int i = 0; i < num; i++) {
        samples[2*i] = results[i].start;
        samples[2*i + 1] = results[i].end;
    } return start;
}
