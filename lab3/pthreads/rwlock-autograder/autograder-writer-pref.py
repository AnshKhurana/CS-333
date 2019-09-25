import os

def reader_acquire_str(id):
    return "Reader: {} has acquired the lock".format(id)

def reader_release_str(id):
    return "Reader: {} has released the lock".format(id)

def writer_acquire_str(id):
    return "Writer: {} has acquired the lock".format(id)

def writer_release_str(id):
    return "Writer: {} has released the lock".format(id)

def find(str, list):
    if str in list:
        return list.index(str)
    else:
        return -1

for file in os.listdir('outputs-writer-pref'):
    with open('outputs-writer-pref/{}'.format(file)) as temp:
        lines = temp.readlines()
    lines = [line.strip() for line in lines]
    file_segments = file.split('_')

    num_readers = int(file_segments[1])
    num_writers = int(file_segments[2])
    if num_readers != 0:
        reader_acquire_indices = [find(reader_acquire_str(id), lines) for id in range(num_readers)]
        reader_release_indices = [find(reader_release_str(id), lines) for id in range(num_readers)]

        if -1 in reader_acquire_indices or -1 in reader_release_indices:
            print("{} FAILED".format(file))
            continue
        if max(reader_acquire_indices) > min(reader_release_indices):
            print("{} FAILED".format(file))
            continue

    if num_writers != 0:
        writer_acquire_indices = [find(writer_acquire_str(id), lines) for id in range(num_writers)]
        writer_release_indices = [find(writer_release_str(id), lines) for id in range(num_writers)]

        if -1 in writer_acquire_indices or -1 in writer_release_indices:
            print("{} FAILED".format(file))
            continue
        for index in range(num_writers):
            if writer_release_indices[index] - writer_acquire_indices[index] != 1:
                print("{} FAILED").format(file)
                break

        if index < num_writers - 1:
            continue

    if num_readers != 0 and num_writers != 0:
        if min(writer_acquire_indices) < max(reader_release_indices):
            print("{} FAILED".format(file))
            continue

    if num_readers != 0:
        reader_acquire_indices = [find(reader_acquire_str(id), lines) for id in range(num_readers, 2*num_readers)]
        reader_release_indices = [find(reader_release_str(id), lines) for id in range(num_readers, 2*num_readers)]

        if -1 in reader_acquire_indices or -1 in reader_release_indices:
            print("{} FAILED".format(file))
            continue

        if max(reader_acquire_indices) > min(reader_release_indices):
            print("{} FAILED".format(file))
            continue
        if num_writers != 0:
            if min(reader_acquire_indices) < max(writer_release_indices):
                print("{} FAILED".format(file))
                continue



    print("{} PASSED".format(file))







