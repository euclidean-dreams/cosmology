#pragma once

#include "axioms.h"

namespace cosmology {

class RingBuffer : public Name {
private:
    std::mutex mutex{};
    std::condition_variable signal_added_expectant{};
    int signal_size;
    int buffer_size;
    vect<float> internal_buffer{};
    vect<float>::const_iterator read_iterator{};
    vect<float>::iterator write_iterator{};

public:
    explicit RingBuffer(int frame_size)
            : signal_size{frame_size},
              buffer_size{signal_size * 16} {
        internal_buffer.resize(buffer_size);
        read_iterator = internal_buffer.cbegin();
        write_iterator = internal_buffer.begin();
    }

    bool next_signal_is_ready() const {
        if (read_iterator + signal_size <= write_iterator
            || (write_iterator < read_iterator && write_iterator >= internal_buffer.begin() + signal_size)) {
            return true;
        } else {
            return false;
        }
    }

    void addSamples(const float *samples, unsigned long count) {
        for (int i = 0; i < count; i++) {
            *write_iterator = samples[i];
            write_iterator++;
        }
        if (write_iterator >= internal_buffer.end()) {
            write_iterator = internal_buffer.begin();
        }
        if (next_signal_is_ready()) {
            signal_added_expectant.notify_all();
        }
    }

    void wait_until_next_signal() {
        std::unique_lock<std::mutex> lock{mutex};
        while (!next_signal_is_ready()) {
            signal_added_expectant.wait(lock);
        }
    }

    uptr<Signal<float>> get_next_signal() {
        auto signal = mkuptr<Signal<float>>();
        for (int i = 0; i < signal_size; i++) {
            signal->push_back(*read_iterator);
            read_iterator++;
        }
        if (read_iterator >= internal_buffer.end()) {
            read_iterator = internal_buffer.cbegin();
        }
        return signal;
    }
};

}
