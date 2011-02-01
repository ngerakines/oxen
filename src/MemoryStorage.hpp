#ifndef __OXEN_MEMORY_STORAGE_H__
#define __OXEN_MEMORY_STORAGE_H__

#include "libtorrent/storage.hpp"

#include <boost/filesystem/path.hpp>

namespace libtorrent {

struct temp_storage : storage_interface {
   temp_storage(file_storage const& fs)
      : m_files(fs)
   {}
   virtual bool initialize(bool /* allocate_files */) { return false; }
   virtual bool has_any_file() { return false; }
   virtual int read(char* buf, int slot, int offset, int size)
   {
      std::map<int, std::vector<char> >::const_iterator i = m_file_data.find(slot);
      if (i == m_file_data.end()) return 0;
      int available = i->second.size() - offset;
      if (available <= 0) return 0;
      if (available > size) available = size;
      memcpy(buf, &i->second[offset], available);
      return available;
   }
   virtual int write(const char* buf, int slot, int offset, int size)
   {
      std::vector<char>& data = m_file_data[slot];
      if ((int) data.size() < offset + size) data.resize(offset + size);
      std::memcpy(&data[offset], buf, size);
      return size;
   }

   virtual bool rename_file(int /* file */, std::string const& /* new_name */) { assert(false); return false; }
   virtual bool move_storage(boost::filesystem::path /* save_path */) { return false; }


   virtual bool verify_resume_data(lazy_entry const& /* rd */, error_code& /* error */) { return false; }
   virtual bool write_resume_data(entry& /* rd */) const { return false; }
   virtual bool move_slot(int /* src_slot */, int /* dst_slot */) { assert(false); return false; }
   virtual bool swap_slots(int /* slot1 */, int /* slot2 */) { assert(false); return false; }
   virtual bool swap_slots3(int /* slot1 */, int /* slot2 */, int /* slot3 */) { assert(false); return false; }
   virtual size_type physical_offset(int slot, int offset) { return slot * m_files.piece_length() + offset; };
   virtual sha1_hash hash_for_slot(int slot, partial_hash& ph, int piece_size)
   {
      int left = piece_size - ph.offset;
      TORRENT_ASSERT(left >= 0);
      if (left > 0)
      {
         std::vector<char>& data = m_file_data[slot];
         // if there are padding files, those blocks will be considered
         // completed even though they haven't been written to the storage.
         // in this case, just extend the piece buffer to its full size
         // and fill it with zeroes.
         if ((int) data.size() < piece_size) data.resize(piece_size, 0);
         ph.h.update(&data[ph.offset], left);
      }
      return ph.h.final();
   }
   virtual bool release_files() { return false; }
   virtual bool delete_files() { return false; }

   std::map<int, std::vector<char> > m_file_data;
   file_storage m_files;
};

storage_interface* temp_storage_constructor(
   file_storage const& fs, file_storage const* /* mapped */
   , boost::filesystem::path const& /* path */, file_pool& /* fp */
   /* , std::vector<boost::uint8_t> const& prio */)
{
   return new temp_storage(fs);
}

}

#endif /* __OXEN_MEMORY_STORAGE_H__ */

