import { createAnalogPanels } from './crt.js';
import { createHudMotion } from './hud-motion.js';

export function createEffects() {
  for (const feature of document.querySelectorAll('.feature')) {
    const template = document.querySelector(`#feature-frame-${feature.dataset.frame}`);
    const frame = template.content.firstElementChild.cloneNode(true);
    feature.prepend(frame);
  }
  for (const frame of document.querySelectorAll('.feature-frame, .section-frame')) {
    const { width: baseWidth, height: baseHeight } = frame.viewBox.baseVal;
    const split = Number(frame.dataset.stretchY);
    const paths = [...frame.querySelectorAll('path')].map(path => [path, path.getAttribute('d')]);
    let resizeFrame;
    new ResizeObserver(([{ contentRect: { width, height } }]) => {
      cancelAnimationFrame(resizeFrame);
      if (!width) return;
      // Defer geometry writes so WebKit can finish delivering resize notifications.
      resizeFrame = requestAnimationFrame(() => {
        if (frame.classList.contains('section-frame')) {
          frame.parentElement.style.minHeight = `${width * baseHeight / baseWidth}px`;
        }
        const frameHeight = Math.round(baseWidth * height / width);
        frame.setAttribute('viewBox', `0 0 ${baseWidth} ${frameHeight}`);
        // Extend the straight rails; preserve the traced corner angles and trim.
        for (const [path, original] of paths) {
          path.setAttribute('d', original.replace(/([ML])([\d.]+) ([\d.]+)/g,
            (_, command, x, y) => `${command}${x} ${Number(y) + (Number(y) > split ? frameHeight - baseHeight : 0)}`));
        }
      });
    }).observe(frame);
  }

  const reducedMotion = matchMedia('(prefers-reduced-motion: reduce)');
  const analogPanels = createAnalogPanels(document.querySelectorAll('.actions, .project-overview, .feature'), reducedMotion);
  const hudMotion = createHudMotion(document.querySelectorAll('.page-header, .actions, .project-overview, .feature'));
  let introAnimations = [];
  const introActive = () => document.documentElement.matches('.intro-pending, .intro-running');

  function wakeScreen(screen) {
    if (introActive() || document.hidden || screen.dataset.crtStarted) return;
    screen.dataset.crtStarted = 'true';
    const panel = screen.querySelector('.game-panel:not([hidden])');
    panel.querySelector('img').decode().then(() => {
      if (!panel.hidden && screen.classList.contains('crt-active')) powerCrt(panel, true);
    }).catch(() => {});
  }

  const crtObserver = new IntersectionObserver(entries => {
    for (const { target, isIntersecting } of entries) {
      target.classList.toggle('crt-active', isIntersecting);
      if (isIntersecting) wakeScreen(target);
    }
  });
  document.querySelectorAll('.game-shot').forEach(screen => {
    crtObserver.observe(screen);
  });

  function updateMotion() {
    analogPanels.refresh();
    hudMotion.refresh(!document.hidden && !reducedMotion.matches && !introActive());
    document.documentElement.classList.toggle('motion-paused', document.hidden);
    if (document.hidden || reducedMotion.matches) {
      finishIntro();
      for (const animation of document.getAnimations()) {
        if (animation.id === 'crt-power') animation.finish();
      }
    }
    if (!document.hidden && !introActive()) {
      document.querySelectorAll('.game-shot.crt-active').forEach(wakeScreen);
    }
  }
  document.addEventListener('visibilitychange', updateMotion);
  reducedMotion.addEventListener('change', updateMotion);
  updateMotion();

  function finishIntro() {
    clearTimeout(window.introFallback);
    document.documentElement.classList.remove('intro-pending', 'intro-running');
    introAnimations.forEach(animation => animation.cancel());
    introAnimations = [];
  }

  function startIntro() {
    if (!introActive()) return;
    const remaining = 3600 - (performance.now() - window.introStartedAt);
    if (remaining <= 0 || reducedMotion.matches || document.hidden) {
      finishIntro();
      updateMotion();
      return;
    }
    // Staged geometry, short signal breaks, then populated panels: https://dlew.me/oblivion
    const rate = remaining / 3600;
    const reveal = (selector, delay, duration, horizontal = false) => {
      const slit = horizontal ? 'inset(0 48% 0 48%)' : 'inset(48% 0 48% 0)';
      for (const element of document.querySelectorAll(selector)) {
        const timing = { id: 'page-intro', delay: delay * rate, duration: duration * rate, fill: 'both' };
        introAnimations.push(element.animate([
          { visibility: 'hidden', clipPath: slit, offset: 0, easing: 'steps(1, end)' },
          { visibility: 'visible', clipPath: slit, offset: .12 },
          { clipPath: slit, offset: .3, easing: 'cubic-bezier(.16, 1, .3, 1)' },
          { visibility: 'visible', clipPath: 'inset(0)', offset: 1 },
        ], timing));
        // Keep opacity pulses separate so the frame keeps opening during each signal break.
        const flicker = [
          { opacity: 0, offset: 0 },
          { opacity: .7, offset: .12 },
          { opacity: .25, offset: .22 },
          { opacity: 1, offset: .3 },
        ];
        const burst = .42 + Math.random() * .08;
        const pulses = duration >= 430 ? 3 : 2;
        for (let pulse = 0; pulse < pulses; pulse++) {
          flicker.push({ opacity: .25, offset: burst + pulse * 60 / duration });
          flicker.push({ opacity: 1, offset: burst + (pulse * 60 + 30) / duration });
        }
        flicker.push({ opacity: 1, offset: 1 });
        introAnimations.push(element.animate(flicker.map(frame => ({ ...frame, easing: 'steps(1, end)' })), timing));
      }
    };
    reveal('.wordmark', 260, 420, true);
    reveal('.edition-label', 570, 460, true);
    reveal('.page-header nav', 960, 360, true);
    reveal('.actions .frame-body, .actions .crt-texture', 650, 850);
    reveal('.actions .frame-indicators', 920, 480, true);
    reveal('.actions .frame-bottom', 1120, 480, true);
    reveal('#patcher-title', 1170, 400, true);
    reveal('#drop-zone', 1510, 490, true);
    reveal('#patch-button', 1810, 410, true);
    reveal('.message-panel', 2160, 300, true);
    reveal('.actions .panel-markings', 2320, 240, true);
    reveal('.overview-frame, .project-overview .crt-texture', 2170, 550);
    reveal('#about-title', 2320, 380, true);
    reveal('.project-intro', 2530, 440);
    reveal('.project-overview .panel-markings', 2650, 240, true);
    for (let index = 1; index <= document.querySelectorAll('.feature').length; index++) {
      const feature = `.feature:nth-child(${index})`;
      reveal(`${feature} .feature-frame, ${feature} .crt-texture`, 2490 + index * 80, 400);
      reveal(`${feature} .game-shot`, 2730 + index * 80, 350);
      reveal(`${feature} .feature-copy`, 2820 + index * 80, 340);
      reveal(`${feature} .edition-tabs`, 2930 + index * 80, 300, true);
      reveal(`${feature} .panel-markings`, 2800 + index * 80, 240, true);
    }
    reveal('footer', 3300, 300);
    for (const [index, joint] of document.querySelectorAll('.joint-cap svg').entries()) {
      const delay = joint.closest('.edition-label') ? 800 : 1370;
      introAnimations.push(joint.animate([
        { transform: `rotate(${index % 2 ? 160 : -160}deg) scale(.7)` },
        { transform: 'rotate(0deg) scale(1)' },
      ], { id: 'page-intro', delay: delay * rate, duration: 520 * rate, fill: 'both', easing: 'cubic-bezier(.2, .8, .2, 1)' }));
    }
    document.documentElement.classList.replace('intro-pending', 'intro-running');
    Promise.allSettled(introAnimations.map(animation => animation.finished)).then(() => {
      finishIntro();
      updateMotion();
    });
  }

  function powerCrt(panel, turnOn) {
    for (const animation of panel.getAnimations({ subtree: true })) {
      if (animation.id === 'crt-power') animation.cancel();
    }
    if (document.hidden || reducedMotion.matches) return null;
    // Android 4.0: separate RGB collapse, white highlight, then a one-pixel line shrinking shut.
    // https://android.googlesource.com/platform/frameworks/base/+/android-4.0.4_r2.1/services/surfaceflinger/SurfaceFlinger.cpp
    const curve = (value, slope) => ((1 / (1 + Math.exp((.5 - value) * slope))) - .5) * (1 + Math.exp(-slope / 2)) + .5;
    const frames = turnOn ? 12 : 24;
    const split = turnOn ? 2 / 3 : .5;
    const lineHeight = 1 / panel.clientHeight;
    const colors = ['red', 'green', 'blue'];
    const channelFrames = colors.map(() => []);
    const beamFrames = [];
    for (let frame = 0; frame <= frames; frame++) {
      const offset = frame / frames;
      const vertical = turnOn ? offset >= split : offset < split;
      const stretch = vertical
        ? (turnOn ? (1 - offset) / (1 - split) : offset / split)
        : (turnOn ? 1 - offset / split : (offset - split) / (1 - split));
      const scales = colors.map((_, index) => curve(stretch, 7.5 + index * .5));
      for (const [index, scale] of scales.entries()) {
        channelFrames[index].push({ offset, opacity: vertical ? 1 : 0,
          transform: `scale(${1 + scale}, ${1 - scale})` });
      }
      const width = vertical ? 1 + scales[2] : 1 - scales[1];
      const height = vertical ? Math.max(lineHeight, 1 - scales[2]) : lineHeight;
      beamFrames.push({ offset, transform: `scale(${width}, ${height})`,
        opacity: vertical ? (turnOn ? 0 : scales[1]) : 1 - scales[1] });
    }
    const layer = document.createElement('div');
    layer.className = 'crt-beam';
    layer.setAttribute('aria-hidden', 'true');
    const timing = { duration: 256, easing: 'linear', fill: 'both' };
    const parts = [];
    for (const [index, color] of colors.entries()) {
      const channel = document.createElement('img');
      channel.src = panel.querySelector('img').currentSrc;
      channel.alt = '';
      channel.style.filter = `url(#crt-${color})`;
      layer.append(channel);
      parts.push(channel.animate(channelFrames[index], timing));
    }
    const beam = document.createElement('span');
    beam.className = 'crt-beam-light';
    layer.append(beam);
    parts.push(beam.animate(beamFrames, timing));
    panel.append(layer);
    const animation = layer.animate([{ opacity: 1 }, { opacity: 1 }], { ...timing, id: 'crt-power' });
    const cleanup = () => {
      parts.forEach(part => part.cancel());
      layer.remove();
      animation.cancel();
    };
    animation.finished.then(cleanup, cleanup);
    return animation;
  }

  analogPanels.prepare().then(startIntro).catch(finishIntro);
  return { powerCrt };
}
