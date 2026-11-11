/* emacsOS kernel interface.  */

#include <config.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "lisp.h"
#include "emacsos-abi.h"

static void
emacsos_control (long operation, uint64_t value, void *reply, size_t size)
{
  const struct emacsos_request request = {
    { EMACSOS_ABI_VERSION, sizeof request }, value
  };
  long result = syscall (EMACSOS_SYS_CONTROL, operation, &request,
                         sizeof request, reply, size);
  if (result < 0)
    error ("emacsOS kernel call: %s", strerror (errno));
  const struct emacsos_header *header = reply;
  if (result != size || header->size != size
      || header->version != EMACSOS_ABI_VERSION)
    error ("Unexpected emacsOS kernel reply");
}

static Lisp_Object
emacsos_system_info (void)
{
  struct emacsos_system_info info;
  emacsos_control (EMACSOS_SYSTEM_INFO, 0, &info, sizeof info);
  if (info.timer_count > EMACSOS_MAX_TIMERFDS)
    error ("Invalid emacsOS timer count");

  Lisp_Object timers = Qnil;
  for (uint32_t i = info.timer_count; i > 0; i--)
    {
      const struct emacsos_timer_info *timer = &info.timers[i - 1];
      Lisp_Object fields[] = {
        intern_c_string (":fd"), make_uint (timer->fd),
        intern_c_string (":clock"),
        intern_c_string (timer->clock == 0 ? "realtime" : "monotonic"),
        intern_c_string (":armed"), timer->armed ? Qt : Qnil,
        intern_c_string (":ready"), timer->ready ? Qt : Qnil,
        intern_c_string (":remaining-ns"), make_uint (timer->remaining_ns),
        intern_c_string (":interval-ns"), make_uint (timer->interval_ns)
      };
      timers = Fcons (Flist (ARRAYELTS (fields), fields), timers);
    }
  Lisp_Object fields[] = {
    intern_c_string (":uptime-ns"), make_uint (info.uptime_ns),
    intern_c_string (":configured-ram-bytes"), make_uint (info.configured_ram_bytes),
    intern_c_string (":mmap-capacity-bytes"), make_uint (info.mmap_capacity_bytes),
    intern_c_string (":mmap-used-bytes"), make_uint (info.mmap_used_bytes),
    intern_c_string (":serial-rx-bytes"), make_uint (info.serial_rx_bytes),
    intern_c_string (":serial-tx-bytes"), make_uint (info.serial_tx_bytes),
    intern_c_string (":pit-ticks"), make_uint (info.pit_ticks),
    intern_c_string (":ide-sectors-read"), make_uint (info.ide_sectors_read),
    intern_c_string (":ramfs-entries-used"), make_uint (info.ramfs_entries_used),
    intern_c_string (":ramfs-entry-limit"), make_uint (info.ramfs_entry_limit),
    intern_c_string (":ramfs-entry-capacity"), make_uint (info.ramfs_entry_capacity),
    intern_c_string (":timerfds"), timers
  };
  return Flist (ARRAYELTS (fields), fields);
}

DEFUN ("emacsos--kernel-call", Femacsos_kernel_call, Semacsos_kernel_call,
       1, 2, 0,
       doc: /* Perform an emacsOS kernel OPERATION, optionally passing VALUE.  */)
  (Lisp_Object operation, Lisp_Object value)
{
  if (EQ (operation, intern_c_string ("system-info")))
    {
      if (!NILP (value))
        error ("system-info takes no value");
      return emacsos_system_info ();
    }
  if (EQ (operation, intern_c_string ("set-ramfs-entry-limit")))
    {
      CHECK_FIXNUM (value);
      if (XFIXNUM (value) < 0)
        error ("RAM filesystem entry limit must be nonnegative");
      struct emacsos_limit_reply reply;
      emacsos_control (EMACSOS_SET_RAMFS_ENTRY_LIMIT, XFIXNUM (value),
                       &reply, sizeof reply);
      return make_uint (reply.limit);
    }
  error ("Unknown emacsOS kernel operation");
}


void
syms_of_emacsos (void)
{
  defsubr (&Semacsos_kernel_call);
}
