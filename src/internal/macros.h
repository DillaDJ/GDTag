#pragma once

#define SNAME(m_arg) ([]() -> const StringName & { static StringName sname = StringName(m_arg, true); return sname; })()