#!/usr/bin/env python
#
# Python-bindings logical_volume type test script
#
# Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import argparse
import os
import random
import sys
import unittest

import pyfvde


class DataRangeFileObject(object):
  """File-like object that maps an in-file data range."""

  def __init__(self, path, range_offset, range_size):
    """Initializes a file-like object.

    Args:
      path (str): path of the file that contains the data range.
      range_offset (int): offset where the data range starts.
      range_size (int): size of the data range starts, or None to indicate
          the range should continue to the end of the parent file-like object.
    """
    super(DataRangeFileObject, self).__init__()
    self._current_offset = 0
    self._file_object = open(path, "rb")
    self._range_offset = range_offset
    self._range_size = range_size

  def __enter__(self):
    """Enters a with statement."""
    return self

  def __exit__(self, unused_type, unused_value, unused_traceback):
    """Exits a with statement."""
    return

  def close(self):
    """Closes the file-like object."""
    if self._file_object:
      self._file_object.close()
      self._file_object = None

  def get_offset(self):
    """Retrieves the current offset into the file-like object.

    Returns:
      int: current offset in the data range.
    """
    return self._current_offset

  def get_size(self):
    """Retrieves the size of the file-like object.

    Returns:
      int: size of the data range.
    """
    return self._range_size

  def read(self, size=None):
    """Reads a byte string from the file-like object at the current offset.

    The function will read a byte string of the specified size or
    all of the remaining data if no size was specified.

    Args:
      size (Optional[int]): number of bytes to read, where None is all
          remaining data.

    Returns:
      bytes: data read.

    Raises:
      IOError: if the read failed.
    """
    if (self._range_offset < 0 or
        (self._range_size is not None and self._range_size < 0)):
      raise IOError("Invalid data range.")

    if self._current_offset < 0:
      raise IOError(
          "Invalid current offset: {0:d} value less than zero.".format(
              self._current_offset))

    if (self._range_size is not None and
        self._current_offset >= self._range_size):
      return b""

    if size is None:
      size = self._range_size
    if self._range_size is not None and self._current_offset + size > self._range_size:
      size = self._range_size - self._current_offset

    self._file_object.seek(
        self._range_offset + self._current_offset, os.SEEK_SET)

    data = self._file_object.read(size)

    self._current_offset += len(data)

    return data

  def seek(self, offset, whence=os.SEEK_SET):
    """Seeks to an offset within the file-like object.

    Args:
      offset (int): offset to seek to.
      whence (Optional(int)): value that indicates whether offset is an absolute
          or relative position within the file.

    Raises:
      IOError: if the seek failed.
    """
    if self._current_offset < 0:
      raise IOError(
          "Invalid current offset: {0:d} value less than zero.".format(
              self._current_offset))

    if whence == os.SEEK_CUR:
      offset += self._current_offset
    elif whence == os.SEEK_END:
      offset += self._range_size
    elif whence != os.SEEK_SET:
      raise IOError("Unsupported whence.")
    if offset < 0:
      raise IOError("Invalid offset value less than zero.")

    self._current_offset = offset


class LogicalVolumeTypeTests(unittest.TestCase):
  """Tests the logical_volume type."""

  def test_unlock(self):
    """Tests the unlock and is_locked functions."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    fvde_volume = pyfvde.volume()

    with DataRangeFileObject(
        test_source, unittest.offset or 0, None) as file_object:

      fvde_volume = pyfvde.volume()
      fvde_volume.open_file_object(file_object)
      fvde_volume.open_physical_volume_files_as_file_objects([file_object])

      fvde_volume_group = fvde_volume.get_volume_group()
      self.assertIsNotNone(fvde_volume_group)

      if not fvde_volume_group.number_of_logical_volumes:
        raise unittest.SkipTest("source has no logical volumes")

      fvde_logical_volume = fvde_volume_group.get_logical_volume(0)
      self.assertIsNotNone(fvde_logical_volume)

      result = fvde_logical_volume.is_locked()
      if result:
        if unittest.password:
          fvde_logical_volume.set_password(unittest.password)
        if unittest.recovery_password:
          fvde_logical_volume.set_recovery_password(unittest.recovery_password)

        result = fvde_logical_volume.unlock()
        self.assertTrue(result)

        result = fvde_logical_volume.is_locked()

      self.assertFalse(result)

      fvde_volume.close()

  def test_read_buffer(self):
    """Tests the read_buffer function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    fvde_volume = pyfvde.volume()

    with DataRangeFileObject(
        test_source, unittest.offset or 0, None) as file_object:

      fvde_volume = pyfvde.volume()
      fvde_volume.open_file_object(file_object)
      fvde_volume.open_physical_volume_files_as_file_objects([file_object])

      fvde_volume_group = fvde_volume.get_volume_group()
      self.assertIsNotNone(fvde_volume_group)

      if not fvde_volume_group.number_of_logical_volumes:
        raise unittest.SkipTest("source has no logical volumes")

      fvde_logical_volume = fvde_volume_group.get_logical_volume(0)
      self.assertIsNotNone(fvde_logical_volume)

      result = fvde_logical_volume.is_locked()
      if result:
        if unittest.password:
          fvde_logical_volume.set_password(unittest.password)
        if unittest.recovery_password:
          fvde_logical_volume.set_recovery_password(unittest.recovery_password)

        result = fvde_logical_volume.unlock()
        self.assertTrue(result)

        result = fvde_logical_volume.is_locked()

      self.assertFalse(result)

      size = fvde_logical_volume.get_size()

      if size < 4096:
        # Test read without maximum size.
        fvde_logical_volume.seek_offset(0, os.SEEK_SET)

        data = fvde_logical_volume.read_buffer()

        self.assertIsNotNone(data)
        self.assertEqual(len(data), size)

      # Test read with maximum size.
      fvde_logical_volume.seek_offset(0, os.SEEK_SET)

      data = fvde_logical_volume.read_buffer(size=4096)

      self.assertIsNotNone(data)
      self.assertEqual(len(data), min(size, 4096))

      if size > 8:
        fvde_logical_volume.seek_offset(-8, os.SEEK_END)

        # Read buffer on size boundary.
        data = fvde_logical_volume.read_buffer(size=4096)

        self.assertIsNotNone(data)
        self.assertEqual(len(data), 8)

        # Read buffer beyond size boundary.
        data = fvde_logical_volume.read_buffer(size=4096)

        self.assertIsNotNone(data)
        self.assertEqual(len(data), 0)

      # Stress test read buffer.
      fvde_logical_volume.seek_offset(0, os.SEEK_SET)

      remaining_size = size

      for _ in range(1024):
        read_size = int(random.random() * 4096)

        data = fvde_logical_volume.read_buffer(size=read_size)

        self.assertIsNotNone(data)

        data_size = len(data)

        if read_size > remaining_size:
          read_size = remaining_size

        self.assertEqual(data_size, read_size)

        remaining_size -= data_size

        if not remaining_size:
          fvde_logical_volume.seek_offset(0, os.SEEK_SET)

          remaining_size = size

      with self.assertRaises(ValueError):
        fvde_logical_volume.read_buffer(size=-1)

      fvde_volume.close()

  def test_read_buffer_at_offset(self):
    """Tests the read_buffer_at_offset function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    fvde_volume = pyfvde.volume()

    with DataRangeFileObject(
        test_source, unittest.offset or 0, None) as file_object:

      fvde_volume = pyfvde.volume()
      fvde_volume.open_file_object(file_object)
      fvde_volume.open_physical_volume_files_as_file_objects([file_object])

      fvde_volume_group = fvde_volume.get_volume_group()
      self.assertIsNotNone(fvde_volume_group)

      if not fvde_volume_group.number_of_logical_volumes:
        raise unittest.SkipTest("source has no logical volumes")

      fvde_logical_volume = fvde_volume_group.get_logical_volume(0)
      self.assertIsNotNone(fvde_logical_volume)

      result = fvde_logical_volume.is_locked()
      if result:
        if unittest.password:
          fvde_logical_volume.set_password(unittest.password)
        if unittest.recovery_password:
          fvde_logical_volume.set_recovery_password(unittest.recovery_password)

        result = fvde_logical_volume.unlock()
        self.assertTrue(result)

        result = fvde_logical_volume.is_locked()

      self.assertFalse(result)

      size = fvde_logical_volume.get_size()

      # Test normal read.
      data = fvde_logical_volume.read_buffer_at_offset(4096, 0)

      self.assertIsNotNone(data)
      self.assertEqual(len(data), min(size, 4096))

      if size > 8:
        # Read buffer on size boundary.
        data = fvde_logical_volume.read_buffer_at_offset(4096, size - 8)

        self.assertIsNotNone(data)
        self.assertEqual(len(data), 8)

        # Read buffer beyond size boundary.
        data = fvde_logical_volume.read_buffer_at_offset(4096, size + 8)

        self.assertIsNotNone(data)
        self.assertEqual(len(data), 0)

      # Stress test read buffer.
      for _ in range(1024):
        random_number = random.random()

        media_offset = int(random_number * size)
        read_size = int(random_number * 4096)

        data = fvde_logical_volume.read_buffer_at_offset(read_size, media_offset)

        self.assertIsNotNone(data)

        remaining_size = size - media_offset

        data_size = len(data)

        if read_size > remaining_size:
          read_size = remaining_size

        self.assertEqual(data_size, read_size)

        remaining_size -= data_size

        if not remaining_size:
          fvde_logical_volume.seek_offset(0, os.SEEK_SET)

      with self.assertRaises(ValueError):
        fvde_logical_volume.read_buffer_at_offset(-1, 0)

      with self.assertRaises(ValueError):
        fvde_logical_volume.read_buffer_at_offset(4096, -1)

      fvde_volume.close()

  def test_seek_offset(self):
    """Tests the seek_offset function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    fvde_volume = pyfvde.volume()

    with DataRangeFileObject(
        test_source, unittest.offset or 0, None) as file_object:

      fvde_volume = pyfvde.volume()
      fvde_volume.open_file_object(file_object)
      fvde_volume.open_physical_volume_files_as_file_objects([file_object])

      fvde_volume_group = fvde_volume.get_volume_group()
      self.assertIsNotNone(fvde_volume_group)

      if not fvde_volume_group.number_of_logical_volumes:
        raise unittest.SkipTest("source has no logical volumes")

      fvde_logical_volume = fvde_volume_group.get_logical_volume(0)
      self.assertIsNotNone(fvde_logical_volume)

      result = fvde_logical_volume.is_locked()
      if result:
        if unittest.password:
          fvde_logical_volume.set_password(unittest.password)
        if unittest.recovery_password:
          fvde_logical_volume.set_recovery_password(unittest.recovery_password)

        result = fvde_logical_volume.unlock()
        self.assertTrue(result)

        result = fvde_logical_volume.is_locked()

      self.assertFalse(result)

      size = fvde_logical_volume.get_size()

      fvde_logical_volume.seek_offset(16, os.SEEK_SET)

      offset = fvde_logical_volume.get_offset()
      self.assertEqual(offset, 16)

      fvde_logical_volume.seek_offset(16, os.SEEK_CUR)

      offset = fvde_logical_volume.get_offset()
      self.assertEqual(offset, 32)

      fvde_logical_volume.seek_offset(-16, os.SEEK_CUR)

      offset = fvde_logical_volume.get_offset()
      self.assertEqual(offset, 16)

      if size > 16:
        fvde_logical_volume.seek_offset(-16, os.SEEK_END)

        offset = fvde_logical_volume.get_offset()
        self.assertEqual(offset, size - 16)

      fvde_logical_volume.seek_offset(16, os.SEEK_END)

      offset = fvde_logical_volume.get_offset()
      self.assertEqual(offset, size + 16)

      # TODO: change IOError into ValueError
      with self.assertRaises(IOError):
        fvde_logical_volume.seek_offset(-1, os.SEEK_SET)

      # TODO: change IOError into ValueError
      with self.assertRaises(IOError):
        fvde_logical_volume.seek_offset(-32 - size, os.SEEK_CUR)

      # TODO: change IOError into ValueError
      with self.assertRaises(IOError):
        fvde_logical_volume.seek_offset(-32 - size, os.SEEK_END)

      # TODO: change IOError into ValueError
      with self.assertRaises(IOError):
        fvde_logical_volume.seek_offset(0, -1)

      fvde_volume.close()

  def test_get_offset(self):
    """Tests the get_offset function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    fvde_volume = pyfvde.volume()

    with DataRangeFileObject(
        test_source, unittest.offset or 0, None) as file_object:

      fvde_volume = pyfvde.volume()
      fvde_volume.open_file_object(file_object)
      fvde_volume.open_physical_volume_files_as_file_objects([file_object])

      fvde_volume_group = fvde_volume.get_volume_group()
      self.assertIsNotNone(fvde_volume_group)

      if not fvde_volume_group.number_of_logical_volumes:
        raise unittest.SkipTest("source has no logical volumes")

      fvde_logical_volume = fvde_volume_group.get_logical_volume(0)
      self.assertIsNotNone(fvde_logical_volume)

      offset = fvde_logical_volume.get_offset()
      self.assertIsNotNone(offset)

      fvde_volume.close()

  def test_get_identifier(self):
    """Tests the get_identifier function and identifier property."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    fvde_volume = pyfvde.volume()

    with DataRangeFileObject(
        test_source, unittest.offset or 0, None) as file_object:

      fvde_volume = pyfvde.volume()
      fvde_volume.open_file_object(file_object)
      fvde_volume.open_physical_volume_files_as_file_objects([file_object])

      fvde_volume_group = fvde_volume.get_volume_group()
      self.assertIsNotNone(fvde_volume_group)

      if not fvde_volume_group.number_of_logical_volumes:
        raise unittest.SkipTest("source has no logical volumes")

      fvde_logical_volume = fvde_volume_group.get_logical_volume(0)
      self.assertIsNotNone(fvde_logical_volume)

      identifier = fvde_logical_volume.get_identifier()
      self.assertIsNotNone(identifier)

      self.assertIsNotNone(fvde_logical_volume.identifier)

      fvde_volume.close()

  def test_get_name(self):
    """Tests the get_name function and name property."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    fvde_volume = pyfvde.volume()

    with DataRangeFileObject(
        test_source, unittest.offset or 0, None) as file_object:

      fvde_volume = pyfvde.volume()
      fvde_volume.open_file_object(file_object)
      fvde_volume.open_physical_volume_files_as_file_objects([file_object])

      fvde_volume_group = fvde_volume.get_volume_group()
      self.assertIsNotNone(fvde_volume_group)

      if not fvde_volume_group.number_of_logical_volumes:
        raise unittest.SkipTest("source has no logical volumes")

      fvde_logical_volume = fvde_volume_group.get_logical_volume(0)
      self.assertIsNotNone(fvde_logical_volume)

      name = fvde_logical_volume.get_name()
      self.assertIsNotNone(name)

      self.assertIsNotNone(fvde_logical_volume.name)

      fvde_volume.close()

  def test_get_size(self):
    """Tests the get_size function and size property."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    fvde_volume = pyfvde.volume()

    with DataRangeFileObject(
        test_source, unittest.offset or 0, None) as file_object:

      fvde_volume = pyfvde.volume()
      fvde_volume.open_file_object(file_object)
      fvde_volume.open_physical_volume_files_as_file_objects([file_object])

      fvde_volume_group = fvde_volume.get_volume_group()
      self.assertIsNotNone(fvde_volume_group)

      if not fvde_volume_group.number_of_logical_volumes:
        raise unittest.SkipTest("source has no logical volumes")

      fvde_logical_volume = fvde_volume_group.get_logical_volume(0)
      self.assertIsNotNone(fvde_logical_volume)

      size = fvde_logical_volume.get_size()
      self.assertIsNotNone(size)

      self.assertIsNotNone(fvde_logical_volume.size)

      fvde_volume.close()


if __name__ == "__main__":
  argument_parser = argparse.ArgumentParser()

  argument_parser.add_argument(
      "-o", "--offset", dest="offset", action="store", default=None,
      type=int, help="offset of the source file.")

  argument_parser.add_argument(
      "-p", "--password", dest="password", action="store", default=None,
      type=str, help="password to unlock the source file.")

  argument_parser.add_argument(
      "-r", "--recovery-password", "--recovery_password",
      dest="recovery_password", action="store", default=None, type=str,
      help="recovery password to unlock the source file.")

  argument_parser.add_argument(
      "source", nargs="?", action="store", metavar="PATH",
      default=None, help="path of the source file.")

  options, unknown_options = argument_parser.parse_known_args()
  unknown_options.insert(0, sys.argv[0])

  setattr(unittest, "offset", options.offset)
  setattr(unittest, "password", options.password)
  setattr(unittest, "recovery_password", options.recovery_password)
  setattr(unittest, "source", options.source)

  unittest.main(argv=unknown_options, verbosity=2)
