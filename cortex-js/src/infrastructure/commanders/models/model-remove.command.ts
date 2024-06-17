import { CommandRunner, SubCommand } from 'nest-commander';
import { ModelsCliUsecases } from '@commanders/usecases/models.cli.usecases';
import { exit } from 'node:process';

@SubCommand({
  name: 'remove',
  description: 'Remove a model by ID locally.',
  arguments: '<model_id>',
  argsDescription: {
    model_id: 'Model to remove',
  },
})
export class ModelRemoveCommand extends CommandRunner {
  constructor(private readonly modelsCliUsecases: ModelsCliUsecases) {
    super();
  }

  async run(passedParams: string[]): Promise<void> {
    if (passedParams.length === 0) {
      console.error('Model ID is required');
      exit(1);
    }

    await this.modelsCliUsecases.removeModel(passedParams[0]).then(console.log);
  }
}
