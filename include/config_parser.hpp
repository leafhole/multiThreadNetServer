#ifndef  CONFIG_PARSER_HPP
#define  CONFIG_PARSER_HPP

#include <glib.h>


class config_parser
{
public:
    config_parser() : kf(NULL), err(NULL) {}
    config_parser(const char *cfgpath) : kf(NULL), err(NULL) { init(cfgpath); }
    ~config_parser() { finish(); }

    bool err_msg(string& msg)
    {
        if (err)
        {
            msg = err->message;
            g_clear_error(&err);
            err = NULL;
            return true;
        }
        return false;
    }

    int init(const char *cfgpath)
    {
        finish();

        if ((kf = g_key_file_new()) == NULL)
            return -1;

        if (g_key_file_load_from_file(kf, cfgpath, G_KEY_FILE_NONE, &err) == FALSE)
        {
            g_key_file_free(kf);
            kf = NULL;
            return -1;
        }

        return 0;
    }

    bool get_boolean(const char *gname, const char *key, bool& ret, bool defval=false)
    {
        g_clear_error(&err);
        gboolean tmp = g_key_file_get_boolean(kf, gname, key, &err);
        if (err)
        {
            ret = defval;
            return false;
        }
        ret = tmp;
        return true;
    }

    bool get_string(const char *gname, const char *key, string& ret, const char *defval="")
    {
        g_clear_error(&err);
        gchar *tmp = g_key_file_get_string(kf, gname, key, &err);
        if (err)
        {
            ret = defval;
            return false;
        }
        ret = tmp;
        g_free(tmp);
        return true;
    }

    bool get_strings(const char *gname, const char *key, vector<string>& ret)
    {
        g_clear_error(&err);
        gsize n = 0;
        gchar **tmp = g_key_file_get_string_list(kf, gname, key, &n, &err);
        ret.clear();
        if (!tmp)
            return false;
        for (gsize i = 0; i < n; ++i)
            ret.push_back(string(tmp[i]));
        g_strfreev(tmp);
        return true;
    }

    bool get_integer(const char *gname, const char *key, int& ret, int defval=0)
    {
        g_clear_error(&err);
        gint tmp = g_key_file_get_integer(kf, gname, key, &err);
        if (err)
        {
            ret = defval;
            return false;
        }
        ret = tmp;
        return true;
    }

    bool get_integers(const char *gname, const char *key, vector<int>& ret)
    {
        g_clear_error(&err);
        gsize n = 0;
        gint *tmp = g_key_file_get_integer_list(kf, gname, key, &n, &err);
        ret.clear();
        if (!tmp)
            return false;
        for (gsize i = 0; i < n; ++i)
            ret.push_back(tmp[i]);
        g_free(tmp);
        return true;
    }

    bool get_uint64(const char *gname, const char *key, uint64_t& ret, uint64_t defval=0)
    {
        g_clear_error(&err);
        gdouble tmp = g_key_file_get_double(kf, gname, key, &err);
        if (err)
        {
            ret = defval;
            return false;
        }
        ret = (uint64_t)tmp;
        return true;
    }

    bool get_uint64s(const char *gname, const char *key, vector<uint64_t>& ret)
    {
        g_clear_error(&err);
        gsize n = 0;
        gdouble *tmp = g_key_file_get_double_list(kf, gname, key, &n, &err);
        ret.clear();
        if (!tmp)
            return false;
        for (gsize i = 0; i < n; ++i)
            ret.push_back((uint64_t)tmp[i]);
        g_free(tmp);
        return true;
    }

private:
    config_parser(const config_parser& rhs);
    config_parser& operator=(const config_parser& rhs);

    void finish()
    {
        if (kf) g_key_file_free(kf);
        g_clear_error(&err);
    }

private:
    GKeyFile *kf;
    GError  *err;
};

#endif   /* CONFIG_PARSER_HPP */
