import { toArray } from '@aztec/foundation/iterable';

import { expect } from 'chai';

import { Key, Range } from './common.js';
import { AztecAsyncMap, AztecAsyncMultiMap, AztecMap, AztecMultiMap } from './map.js';
import { AztecAsyncKVStore, AztecKVStore } from './store.js';
import { isAsyncStore } from './utils.js';

export function describeAztecMap(testName: string, getStore: () => Promise<AztecKVStore | AztecAsyncKVStore>) {
  describe(testName, () => {
    let store: AztecKVStore | AztecAsyncKVStore;
    let map: AztecMultiMap<Key, string> | AztecAsyncMultiMap<Key, string>;

    beforeEach(async () => {
      store = await getStore();
      map = store.openMultiMap<string | [number, string], string>('test');
    });

    async function get(key: Key, sut: AztecAsyncMap<any, any> | AztecMap<any, any> = map) {
      return isAsyncStore(store)
        ? await (sut as AztecAsyncMultiMap<any, any>).getAsync(key)
        : (sut as AztecMultiMap<any, any>).get(key);
    }

    async function entries() {
      return isAsyncStore(store)
        ? await toArray((map as AztecAsyncMultiMap<any, any>).entriesAsync())
        : await toArray((map as AztecMultiMap<any, any>).entries());
    }

    async function values() {
      return isAsyncStore(store)
        ? await toArray((map as AztecAsyncMultiMap<any, any>).valuesAsync())
        : await toArray((map as AztecMultiMap<any, any>).values());
    }

    async function keys(range?: Range<Key>, sut: AztecAsyncMap<any, any> | AztecMap<any, any> = map) {
      return isAsyncStore(store)
        ? await toArray((sut as AztecAsyncMultiMap<any, any>).keysAsync(range))
        : await toArray((sut as AztecMultiMap<any, any>).keys(range));
    }

    async function getValues(key: Key) {
      return isAsyncStore(store)
        ? await toArray((map as AztecAsyncMultiMap<any, any>).getValuesAsync(key))
        : await toArray((map as AztecMultiMap<any, any>).getValues(key));
    }

    it('should be able to set and get values', async () => {
      await map.set('foo', 'bar');
      await map.set('baz', 'qux');

      expect(await get('foo')).to.equal('bar');
      expect(await get('baz')).to.equal('qux');
      expect(await get('quux')).to.equal(undefined);
    });

    it('should be able to set values if they do not exist', async () => {
      expect(await map.setIfNotExists('foo', 'bar')).to.equal(true);
      expect(await map.setIfNotExists('foo', 'baz')).to.equal(false);

      expect(await get('foo')).to.equal('bar');
    });

    it('should be able to delete values', async () => {
      await map.set('foo', 'bar');
      await map.set('baz', 'qux');

      await map.delete('foo');

      expect(await get('foo')).to.equal(undefined);
      expect(await get('baz')).to.equal('qux');
    });

    it('should be able to iterate over entries when there are no keys', async () => {
      expect(await entries()).to.deep.equal([]);
    });

    it('should be able to iterate over entries', async () => {
      await map.set('foo', 'bar');
      await map.set('baz', 'qux');

      expect(await entries()).to.deep.equal([
        ['baz', 'qux'],
        ['foo', 'bar'],
      ]);
    });

    it('should be able to iterate over values', async () => {
      await map.set('foo', 'bar');
      await map.set('baz', 'quux');

      expect(await values()).to.deep.equal(['quux', 'bar']);
    });

    it('should be able to iterate over keys', async () => {
      await map.set('foo', 'bar');
      await map.set('baz', 'qux');

      expect(await keys()).to.deep.equal(['baz', 'foo']);
    });

    it('should be able to get multiple values for a single key', async () => {
      await map.set('foo', 'bar');
      await map.set('foo', 'baz');

      expect(await getValues('foo')).to.deep.equal(['bar', 'baz']);
    });

    it('should be able to delete individual values for a single key', async () => {
      await map.set('foo', 'bar');
      await map.set('foo', 'baz');

      await map.deleteValue('foo', 'bar');

      expect(await getValues('foo')).to.deep.equal(['baz']);
    });

    it('supports tuple keys', async () => {
      // Use a new map because key structure has changed
      const tupleMap = store.openMap<[number, string], string>('test-tuple');

      await tupleMap.set([5, 'bar'], 'val');
      await tupleMap.set([0, 'foo'], 'val');

      expect(await keys(undefined, tupleMap)).to.deep.equal([
        [0, 'foo'],
        [5, 'bar'],
      ]);

      expect(await get([5, 'bar'], tupleMap)).to.equal('val');
    });

    it('supports range queries', async () => {
      await map.set('a', 'a');
      await map.set('b', 'b');
      await map.set('c', 'c');
      await map.set('d', 'd');

      expect(await keys({ start: 'b', end: 'c' })).to.deep.equal(['b']);
      expect(await keys({ start: 'b' })).to.deep.equal(['b', 'c', 'd']);
      expect(await keys({ end: 'c' })).to.deep.equal(['a', 'b']);
      expect(await keys({ start: 'b', end: 'c', reverse: true })).to.deep.equal(['c']);
      expect(await keys({ start: 'b', limit: 1 })).to.deep.equal(['b']);
      expect(await keys({ start: 'b', reverse: true })).to.deep.equal(['d', 'c']);
      expect(await keys({ end: 'b', reverse: true })).to.deep.equal(['b', 'a']);
    });
  });
}