#ifndef HAUNT_ENTRY_ENGINE_H
#define HAUNT_ENTRY_ENGINE_H

export b8 _engine_init(const App_Config* config);

export b8 _engine_update(void);

export b8 _engine_render(void);

export void _engine_shutdown(void);

export b8 _engine_is_running(void);

#endif // HAUNT_ENTRY_ENGINE_H
