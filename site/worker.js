import { applyBps } from './patcher.mjs';

const ROM_SIZE = 8 * 1024 * 1024;
const ROM_SHA1 = '460fa2158606097f6e6f63ce966d2d6ecdd58d70';

self.onmessage = async ({ data }) => {
  try {
    if (data.rom.size !== ROM_SIZE) throw new Error('Use an unmodified 8 MiB Zoids™ Legacy (USA) ROM.');
    const source = new Uint8Array(await data.rom.arrayBuffer());
    const hash = new Uint8Array(await crypto.subtle.digest('SHA-1', source));
    const sha1 = Array.from(hash, byte => byte.toString(16).padStart(2, '0')).join('');
    if (sha1 !== ROM_SHA1) throw new Error('This is not a clean Zoids™ Legacy (USA) ROM. Choose an unmodified USA copy.');
    if (data.action === 'validate') {
      self.postMessage({ validated: true });
      return;
    }
    const response = await fetch('./patch.bps', { cache: 'no-store' });
    if (!response.ok) throw new Error('The bundled patch is unavailable. Please try again later.');
    const patch = new Uint8Array(await response.arrayBuffer());
    const target = applyBps(source, patch);
    self.postMessage({ target: target.buffer }, [target.buffer]);
  } catch (error) {
    self.postMessage({ error: error.message || 'Patching failed. Please try again.' });
  }
};
