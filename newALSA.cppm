module;
// The global module fragment isolates all messy global macros here
#include <alsa/asoundlib.h>

export module newALSA;
import std;

export namespace alsa::pcm {
    // Type-safe opaque struct types (Zero runtime cost, prevents pointer mismatch bugs)
    using Handle   = struct PcmHandle;
    using HwParams = struct PcmHwParams;
    
    // Modern platform-independent frame measurements
    using UFrames  = unsigned long;
    using SFrames  = long;

    // Type-safe strongly typed modern enums mapping directly to ALSA flags
    enum class StreamType : int { 
        Playback = SND_PCM_STREAM_PLAYBACK, 
        Capture  = SND_PCM_STREAM_CAPTURE 
    };
    
    enum class AccessType : int { 
        RwInterleaved = SND_PCM_ACCESS_RW_INTERLEAVED 
    };
    
    enum class Format : int { 
        S16_LE = SND_PCM_FORMAT_S16_LE, 
        Float  = SND_PCM_FORMAT_FLOAT 
    };

    enum class OpenMode : unsigned {
        Nonblock         = SND_PCM_NONBLOCK,
        Async            = SND_PCM_ASYNC,
        NoAutoResample   = SND_PCM_NO_AUTO_RESAMPLE,
        NoAutoChannels   = SND_PCM_NO_AUTO_CHANNELS,
        NoAutoFormat     = SND_PCM_NO_AUTO_FORMAT,
        NoSoftvol        = SND_PCM_NO_SOFTVOL
    };

    constexpr unsigned to_open_flags(OpenMode mode) noexcept {
        return static_cast<unsigned>(mode);
    }

    // --- High-Performance Inline API ---

    [[nodiscard]] inline int open(Handle** pcm, const char* name, StreamType stream, unsigned mode = 0) noexcept {
        return ::snd_pcm_open(reinterpret_cast<::snd_pcm_t**>(pcm), name, static_cast<::snd_pcm_stream_t>(stream), mode);
    }

    inline int close(Handle* pcm) noexcept {
        return ::snd_pcm_close(reinterpret_cast<::snd_pcm_t*>(pcm));
    }

    [[nodiscard]] inline int hw_params_malloc(HwParams** params) noexcept {
        return ::snd_pcm_hw_params_malloc(reinterpret_cast<::snd_pcm_hw_params_t**>(params));
    }

    inline void hw_params_free(HwParams* params) noexcept {
        ::snd_pcm_hw_params_free(reinterpret_cast<::snd_pcm_hw_params_t*>(params));
    }

    [[nodiscard]] inline int hw_params_any(Handle* pcm, HwParams* params) noexcept {
        return ::snd_pcm_hw_params_any(reinterpret_cast<::snd_pcm_t*>(pcm), reinterpret_cast<::snd_pcm_hw_params_t*>(params));
    }

    [[nodiscard]] inline int hw_params_set_access(Handle* pcm, HwParams* params, AccessType access) noexcept {
        return ::snd_pcm_hw_params_set_access(reinterpret_cast<::snd_pcm_t*>(pcm), reinterpret_cast<::snd_pcm_hw_params_t*>(params), static_cast<::snd_pcm_access_t>(access));
    }

    [[nodiscard]] inline int hw_params_set_format(Handle* pcm, HwParams* params, Format format) noexcept {
        return ::snd_pcm_hw_params_set_format(reinterpret_cast<::snd_pcm_t*>(pcm), reinterpret_cast<::snd_pcm_hw_params_t*>(params), static_cast<::snd_pcm_format_t>(format));
    }

    [[nodiscard]] inline int hw_params_set_channels(Handle* pcm, HwParams* params, unsigned int channels) noexcept {
        return ::snd_pcm_hw_params_set_channels(reinterpret_cast<::snd_pcm_t*>(pcm), reinterpret_cast<::snd_pcm_hw_params_t*>(params), channels);
    }

    [[nodiscard]] inline int hw_params_set_rate_near(Handle* pcm, HwParams* params, unsigned int* rate, int* dir) noexcept {
        return ::snd_pcm_hw_params_set_rate_near(reinterpret_cast<::snd_pcm_t*>(pcm), reinterpret_cast<::snd_pcm_hw_params_t*>(params), rate, dir);
    }

    [[nodiscard]] inline int hw_params_apply(Handle* pcm, HwParams* params) noexcept {
        return ::snd_pcm_hw_params(reinterpret_cast<::snd_pcm_t*>(pcm), reinterpret_cast<::snd_pcm_hw_params_t*>(params));
    }

    [[nodiscard]] inline int hw_params_get_period_size(const HwParams* params, UFrames* frames, int* dir) noexcept {
        return ::snd_pcm_hw_params_get_period_size(reinterpret_cast<const ::snd_pcm_hw_params_t*>(params), frames, dir);
    }

    [[nodiscard]] inline int set_params(
        Handle* pcm, Format format, AccessType access, unsigned int channels, 
        unsigned int rate, int soft_resample, unsigned int latency) noexcept 
    {
        return ::snd_pcm_set_params(
            reinterpret_cast<::snd_pcm_t*>(pcm), 
            static_cast<::snd_pcm_format_t>(format), 
            static_cast<::snd_pcm_access_t>(access), 
            channels, rate, soft_resample, latency
        );
    }

    [[nodiscard]] inline int get_params(Handle* pcm, UFrames* buffer_size, UFrames* period_size) noexcept {
        return ::snd_pcm_get_params(reinterpret_cast<::snd_pcm_t*>(pcm), buffer_size, period_size);
    }

    [[nodiscard]] inline SFrames write_interleaved(Handle* pcm, const void* buffer, UFrames frames) noexcept {
        return ::snd_pcm_writei(reinterpret_cast<::snd_pcm_t*>(pcm), buffer, frames);
    }

    [[nodiscard]] inline SFrames read_interleaved(Handle* pcm, void* buffer, UFrames frames) noexcept {
        return ::snd_pcm_readi(reinterpret_cast<::snd_pcm_t*>(pcm), buffer, frames);
    }

    [[nodiscard]] inline int recover(Handle* pcm, int err, bool silent = false) noexcept {
        return ::snd_pcm_recover(reinterpret_cast<::snd_pcm_t*>(pcm), err, silent ? 1 : 0);
    }

    [[nodiscard]] inline int prepare(Handle* pcm) noexcept { return ::snd_pcm_prepare(reinterpret_cast<::snd_pcm_t*>(pcm)); }
    [[nodiscard]] inline int drain(Handle* pcm)   noexcept { return ::snd_pcm_drain(reinterpret_cast<::snd_pcm_t*>(pcm)); }
    [[nodiscard]] inline int drop(Handle* pcm)    noexcept { return ::snd_pcm_drop(reinterpret_cast<::snd_pcm_t*>(pcm)); }
}

export namespace alsa {
    [[nodiscard]] inline const char* strerror(int errnum) noexcept { 
        return ::snd_strerror(errnum); 
    }
}
