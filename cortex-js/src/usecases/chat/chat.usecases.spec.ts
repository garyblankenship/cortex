import { Test, TestingModule } from '@nestjs/testing';
import { ChatUsecases } from './chat.usecases';
import { DatabaseModule } from '@/infrastructure/database/database.module';
import { ExtensionModule } from '@/infrastructure/repositories/extensions/extension.module';
import { ModelRepositoryModule } from '@/infrastructure/repositories/models/model.module';
import { HttpModule } from '@nestjs/axios';
import { DownloadManagerModule } from '@/download-manager/download-manager.module';
import { EventEmitterModule } from '@nestjs/event-emitter';

describe('ChatService', () => {
  let service: ChatUsecases;

  beforeEach(async () => {
    const module: TestingModule = await Test.createTestingModule({
      imports: [
        DatabaseModule,
        ExtensionModule,
        ModelRepositoryModule,
        HttpModule,
        DownloadManagerModule,
        EventEmitterModule.forRoot(),
      ],
      providers: [ChatUsecases],
      exports: [ChatUsecases],
    }).compile();

    service = module.get<ChatUsecases>(ChatUsecases);
  });

  it('should be defined', () => {
    expect(service).toBeDefined();
  });
});
