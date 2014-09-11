import gevent
from gevent.queue import Queue

tasks = Queue()

def worker(user):
    while not tasks.empty():
        task = tasks.get()
        print('%s got task %s' % (user, task))
        gevent.sleep(0)
    print('Quitting worker!')

def boss():
    for i in xrange(4):
        tasks.put_nowait(i)

gevent.spawn(boss).join()

gevent.joinall([
    gevent.spawn(worker, 'steve'),
    gevent.spawn(worker, 'john'),
])
