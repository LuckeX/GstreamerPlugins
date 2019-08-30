'use strict';

const log = require('../logger').logger.getLogger('AnalyticsAgent');
const BaseAgent = require('./base-agent');

const VideoPipeline = require('../videoGstPipeline/build/Release/videoAnalyzer-pipeline');

const MediaFrameMulticaster = require('../mediaFrameMulticaster/build/Release/mediaFrameMulticaster');
const EventEmitter = require('events').EventEmitter;
const { getVideoParameterForAddon } = require('../mediaUtil');

var portInfo = 0; 
var count = 0;


class AnalyticsAgent  {
  constructor(config) {
    this.algorithms = config.algorithms;
    this.onStatus = config.onStatus;
    this.onStreamGenerated = config.onStreamGenerated;
    this.onStreamDestroyed = config.onStreamDestroyed;

    this.agentId = config.agentId;
    this.rpcId = config.rpcId;
    this.outputs = {}

    var conf = {
      'hardware': false,
      'simulcast': false,
      'crop': false,
      'gaccplugin': false,
      'MFE_timeout': 0
    };
    this.engine = new VideoPipeline(conf);

    this.flag = 0;
  }

// override
createInternalConnection(connectionId, direction, internalOpt) {
    internalOpt.minport = global.config.internal.minport;
    internalOpt.maxport = global.config.internal.maxport;
    log.info('================direction:', direction);
    if(direction == 'in'){
      this.engine.emit_ListenTo(internalOpt.minport,internalOpt.maxport);
      portInfo = this.engine.getListeningPort();
      log.info('portinfo:', portInfo);
    }
    
    // Create internal connection always success
    return Promise.resolve({ip: global.config.internal.ip_address, port: portInfo});
  }

  // override
  publish(connectionId, connectionType, options) {
    log.debug('publish:', connectionId, connectionType, options);
    if (connectionType !== 'analytics') {
      return Promise.resolve("ok");
    }
    // should not be reached
    return Promise.reject('no analytics publish');
  }

  // override
  unpublish(connectionId) {
    log.debug('unpublish:', connectionId);
    return Promise.resolve();
  }

  // override
  subscribe(connectionId, connectionType, options) { 
    log.debug('subscribe:', connectionId, connectionType, JSON.stringify(options));
    if (connectionType !== 'analytics') {
       // return super.subscribe(connectionId, connectionType, options);
       if(!this.outputs[connectionId]){
        this.outputs[connectionId] = count;
        log.debug('====subscribe:', connectionId, count, this.outputs[connectionId]);
        this.engine.stopLoop();
        this.engine.emit_ConnectTo(count,options.ip,options.port);
        count++;
       }
      return Promise.resolve("ok");
    }

      this.engine.createPipeline();
      const videoFormat = options.connection.video.format;
      const videoParameters = options.connection.video.parameters;
      const algo = options.connection.algorithm;
      const status = {type: 'ready', info: {algorithm: algo}};
      this.onStatus(options.controller, connectionId, 'out', status);

      const newStreamId = Math.random() * 1000000000000000000 + '';
      log.info('new stream added', newStreamId);
      const streamInfo = {
          type: 'analytics',
          media: {video: Object.assign({}, videoFormat, videoParameters)},
          analyticsId: connectionId,
          locality: {agent:this.agentId, node:this.rpcId},
        };
      log.info('agent:',this.agentId,'node:',this.rpcId);

      const pluginName = this.algorithms[algo].name;
      let codec = videoFormat.codec;
            if (videoFormat.profile) {
              codec += '_' + videoFormat.profile;
            }
      codec = codec.toLowerCase();
      const {resolution, framerate, keyFrameInterval, bitrate}
              = getVideoParameterForAddon(options.connection.video);

      log.info('resolution:',resolution,'framerate:',framerate,'keyFrameInterval:',
               keyFrameInterval, 'bitrate:',bitrate);
      
      this.engine.setOutputParam(codec,resolution,framerate,bitrate,keyFrameInterval,algo,pluginName);

      streamInfo.media.video.bitrate = bitrate;
            this.onStreamGenerated(options.controller, newStreamId, streamInfo);

      this.engine.addElementMany();
      return Promise.resolve();
  }

  // override
  unsubscribe(connectionId) {
    log.debug('unsubscribe:', connectionId);
    this.engine.disconnect(this.outputs[connectionId]);
    return Promise.resolve();
  }

  // override
  linkup(connectionId, audioFrom, videoFrom) {
    log.debug('linkup:', connectionId, audioFrom, videoFrom);
    this.engine.setPlaying();

    return Promise.resolve();
  }

  // override
  cutoff(connectionId) {
    log.debug('cutoff:', connectionId);
    return Promise.resolve();
  }

  cleanup() {
    log.debug('cleanup');
    return Promise.resolve();
  }
}


module.exports = AnalyticsAgent;
